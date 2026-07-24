#include "core/Animation.hpp"
#include "core/AnimationPlayer.hpp"
#include "core/Pet.hpp"
#include "screen/ui/widgets/CenteredLayout.hpp"
#include "screen/ui/widgets/GitCard.hpp"
#include <chrono>
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/terminal.hpp>
#include <git/GitLibrary.hpp>
#include <git/Repository.hpp>
#include <git2.h>
#include <git2/repository.h>

#include <algorithm>
#include <cctype>
#include <string>
#include <thread>
#define STB_IMAGE_IMPLEMENTATION
using namespace ftxui;
using namespace std;
using namespace std::chrono_literals;

std::string toLower(std::string str) {
  std::transform(str.begin(), str.end(), str.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return str;
}

int main() {
  GitLibrary git;

  std::filesystem::path path = std::filesystem::current_path();
  Repository repo(path);

  PetStage stage = PetStage::Egg;
  int commitCount = repo.commitCount();
  if (commitCount > 100) {
    stage = PetStage::Adult;
  } else if (commitCount > 50 && commitCount <= 100) {
    stage = PetStage::Teen;
  } else if (commitCount > 20 && commitCount <= 50) {
    stage = PetStage::Baby;
  } else {
    stage = PetStage::Egg;
  }

  filesystem::path egg_path("assets/egg");
  filesystem::path baby_path("assets/baby");
  filesystem::path egg_animation_path = path / egg_path;
  filesystem::path baby_animation_path = path / baby_path;
  Animation eggAnimation = Animation::Load(egg_animation_path);
  Animation babyAnimation = Animation::Load(baby_animation_path);
  AnimationPlayer animationPlayer;

  animationPlayer.add("egg", eggAnimation);
  animationPlayer.add("baby", babyAnimation);

  Pet pet("Milo", animationPlayer, stage);

  pet.animationPlayer().play(toLower(pet.stage()));

  int active_view = 0;
  bool show_menu = false;
  int selected_menu_item = 0;

  auto screen = ScreenInteractive::TerminalOutput();

  auto component = Renderer([&] {
    Element card =
        GitCard(pet, repo, active_view, show_menu, selected_menu_item);
    // Constrain the card to a fixed size of 80x25
    card = card | size(WIDTH, EQUAL, 80) | size(HEIGHT, EQUAL, 25);
    return CenteredLayout(std::move(card));
  });

  std::atomic<bool> running = true;

  // Run a thread to emit event to update pet animation
  std::thread animation_thread([&] {
    while (running) {
      std::this_thread::sleep_for(16ms);
      screen.PostEvent(Event::Custom);
    }
  });

  auto last = std::chrono::steady_clock::now();
  component = CatchEvent(component, [&](Event event) {
    if (event == Event::Tab) {
      show_menu = !show_menu;
      if (show_menu) {
        selected_menu_item = active_view;
      }
      return true;
    }
    if (show_menu) {
      if (event == Event::ArrowUp) {
        selected_menu_item = (selected_menu_item - 1 + 3) % 3;
        return true;
      }
      if (event == Event::ArrowDown) {
        selected_menu_item = (selected_menu_item + 1) % 3;
        return true;
      }
      if (event == Event::Return) {
        active_view = selected_menu_item;
        show_menu = false;
        return true;
      }
      if (event == Event::Escape) {
        show_menu = false;
        return true;
      }
    }
    if (event == Event::Character('q') || event == Event::Escape) {
      screen.ExitLoopClosure()();
      return true;
    }
    if (event == Event::Custom) {
      auto now = std::chrono::steady_clock::now();
      auto dt =
          std::chrono::duration_cast<std::chrono::milliseconds>(now - last);
      last = now;

      // pet.update(dt);
      pet.animationPlayer().update(dt);
      return true;
    }
    return false;
  });

  screen.Loop(component);
  animation_thread.detach();

  return 0;
}
