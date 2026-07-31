#include "core/Animation.hpp"
#include "core/AnimationPlayer.hpp"
#include "core/Config.hpp"
#include "core/Pet.hpp"
#include "screen/ui/widgets/AnimatedComponent.hpp"
#include "screen/ui/widgets/CenteredLayout.hpp"
#include "screen/ui/widgets/GitCard.hpp"
#include <chrono>
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/animation.hpp>
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
#include <csignal>
#include <iostream>
#include <ostream>
#include <string>

using namespace ftxui;
using namespace std;
using namespace std::chrono_literals;

namespace {
void cleanupTerminal() {
  // Disable mouse tracking (1000, 1002, 1003, 1006)
  std::cerr << "\033[?1000l\033[?1002l\033[?1003l\033[?1006l";
  // Show cursor
  std::cerr << "\033[?25h";
  // Exit alternate screen buffer
  std::cerr << "\033[?1049l";
  std::cerr << std::flush;
}

void signalHandler(int signal) {
  cleanupTerminal();
  std::signal(signal, SIG_DFL);
  std::raise(signal);
}
} // namespace

std::string toLower(std::string str) {
  std::transform(str.begin(), str.end(), str.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return str;
}

int main() {
  try {
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

    LocalConfig localconfig = LocalConfig::Load(repo);
    PetConfig petconfig = PetConfig::Load(localconfig.id());

    PetMood initialMood = PetMood::Neutral;
    if (repo.hasConflicts()) {
      initialMood = PetMood::Angry;
    } else if (repo.commitCountPerWeek() >= 20 ||
               repo.commitCountPerDay() >= 10) {
      initialMood = PetMood::Happy;
    } else if (repo.commitCountPerWeek() <= 5) {
      initialMood = PetMood::Sad;
    }

    Pet pet = Pet::Load(localconfig, petconfig, stage, initialMood);

    int active_view = 0;
    bool show_menu = false;
    int selected_menu_item = 0;

    auto screen = ScreenInteractive::TerminalOutput();

    // Create a custom component that uses FTXUI's built-in animation system.
    // OnAnimation runs on the main thread — no background thread needed.
    auto last = animation::Clock::now();
    auto render_component = Renderer([&] {
      PetMood mood = PetMood::Neutral;
      if (repo.hasConflicts()) {
        mood = PetMood::Angry;
      } else if (repo.commitCountPerWeek() >= 20 ||
                 repo.commitCountPerDay() >= 10) {
        mood = PetMood::Happy;
      } else if (repo.commitCountPerWeek() <= 5) {
        mood = PetMood::Sad;
      }
      pet.setMood(mood);

      // Request the next animation frame so OnAnimation keeps being called.
      animation::RequestAnimationFrame();

      Element card =
          GitCard(pet, repo, active_view, show_menu, selected_menu_item);
      // Constrain the card to a fixed size of 80x25
      card = card | size(WIDTH, EQUAL, 80) | size(HEIGHT, EQUAL, 25);
      return CenteredLayout(std::move(card));
    });

    auto component = CatchEvent(render_component, [&](Event event) {
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
        screen.Exit();
        return true;
      }
      return false;
    });

    auto animated = Make<AnimatedComponent>(component, pet);

    // Request the first animation frame to kick off the loop.
    animation::RequestAnimationFrame();

    screen.Loop(animated);

    return 0;
  } catch (const std::exception &e) {
    std::cerr << "\n==================================================\n"
              << "Error during initialization:\n"
              << e.what() << "\n"
              << "==================================================\n"
              << std::endl;
    return 1;
  }
}
