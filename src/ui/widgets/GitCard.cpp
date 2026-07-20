#include <format>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <screen/ui/widgets/GitCard.hpp>

using namespace ftxui;
using namespace std;
Element Pixel(Color color) { return text("  ") | bgcolor(color); }
Element GitCard(Pet &pet, const Repository &repo) {
  Branch branch = repo.currentBranch();
  int commitCount = repo.commitCount();
  Commit head = repo.head();

  Element pet_element = vbox({
      hbox({
          Pixel(Color::Black),
          Pixel(Color::Red),
          Pixel(Color::Red),
          Pixel(Color::Black),
      }),
      hbox({
          Pixel(Color::Red),
          Pixel(Color::White),
          Pixel(Color::White),
          Pixel(Color::Red),
      }),
      hbox({
          Pixel(Color::Black),
          Pixel(Color::Red),
          Pixel(Color::Red),
          Pixel(Color::Black),
      }),
  });

  Element petStats = vflow({
      text(format("Pet: {}", pet.name())),
      text(format("Stage: {}", pet.stage())),
  });

  Element repoInfo = vflow({text(format("Repository: {}", repo.name()))});

  Element gitStats = vflow({
      text(format("Current Branch: {}", branch.shortname())),
      text(format("Total Commit: {}", commitCount)),
      text(format("Last Commit: {}", head.sorthash())),
  });
  Element card =
      vflow({filler() | flex, center(pet_element) | flex, filler() | flex,
             petStats, filler() | flex, repoInfo, filler() | flex, gitStats}) |
      border;

  return card;
}
