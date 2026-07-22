#include "git/Branch.hpp"
#include "screen/ui/widgets/CenteredLayout.hpp"
#include "screen/ui/widgets/GitCard.hpp"
#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/terminal.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <git/GitLibrary.hpp>
#include <git/Repository.hpp>
#include <git2.h>
#include <git2/repository.h>
#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
using namespace ftxui;
using namespace std;

std::string exec(const std::string &cmd) {
  char buffer[128];
  std::string result;

  FILE *pipe = popen(cmd.c_str(), "r");
  if (!pipe)
    return "";

  while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
    result += buffer;
  }

  pclose(pipe);
  return result;
}

int main() {
  GitLibrary git;

  std::filesystem::path path = std::filesystem::current_path();
  Repository repo(path);

  Pet pet("Milo");

  filesystem::path pet_path("assets/egg.png");
  filesystem::path combined_path = path / pet_path;
  Image image = Image::Load(combined_path);

  int active_view = 0;
  bool show_menu = false;
  int selected_menu_item = 0;

  auto screen = ScreenInteractive::TerminalOutput();

  auto component = Renderer([&] {
    Element card = GitCard(pet, repo, image, active_view, show_menu, selected_menu_item);
    // Constrain the card to a fixed size of 80x25
    card = card | size(WIDTH, EQUAL, 80) | size(HEIGHT, EQUAL, 25);
    return CenteredLayout(std::move(card));
  });

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
    return false;
  });

  screen.Loop(component);

  return 0;
}
