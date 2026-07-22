#include <screen/ui/widgets/BranchesView.hpp>
#include <screen/ui/widgets/CommitsView.hpp>
#include <screen/ui/widgets/GitCard.hpp>
#include <screen/ui/widgets/MainView.hpp>
#include <screen/ui/widgets/PetView.hpp>
#include <screen/ui/widgets/RepoInfo.hpp>

using namespace ftxui;

Element GitCard(Pet &pet, const Repository &repo, int activeView, bool showMenu,
                int selectedMenuItem) {
  Element left_column = PetView(pet);

  Element content_pane;
  if (showMenu) {
    content_pane = vbox({
        text("Navigation Menu") | bold | color(Color::Magenta) | hcenter,
        separator(),
        filler() | flex,
        vbox({
            (selectedMenuItem == 0)
                ? (text("> Main Info   <") | bold | color(Color::Green))
                : (text("  Main Info    ") | dim),
            (selectedMenuItem == 1)
                ? (text("> Commit Info <") | bold | color(Color::Cyan))
                : (text("  Commit Info  ") | dim),
            (selectedMenuItem == 2)
                ? (text("> Branch Info <") | bold | color(Color::Yellow))
                : (text("  Branch Info  ") | dim),
        }) | hcenter,
        filler() | flex,
        separator(),
        text("Up/Down: Navigate | Enter: Choose") | dim | hcenter,
    });
  } else {
    Element active_view_el;
    if (activeView == 0) {
      active_view_el = MainView(repo);
    } else if (activeView == 1) {
      active_view_el = CommitsView(repo);
    } else {
      active_view_el = BranchesView(repo);
    }

    content_pane = vbox({
        active_view_el,
        filler() | flex,
        separator(),
        text("Press [Tab] to change view") | dim | hcenter,
    });
  }

  Element right_column = vbox({
      RepoInfo(repo),
      separator(),
      content_pane | flex,
  });

  Element card = hbox({
                     left_column | size(WIDTH, EQUAL, 36),
                     separator(),
                     right_column | flex,
                 }) |
                 border;

  return card;
}
