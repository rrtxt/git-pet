#include <format>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <screen/ui/widgets/GitCard.hpp>

using namespace ftxui;
using namespace std;
Element Pixel(Color color) { return text("  ") | bgcolor(color); }
Element GitCard(const Repository &repo) {
  Branch branch = repo.currentBranch();
  int commitCount = repo.commitCount();
  Commit head = repo.head();

  Canvas c(40, 20);

  c.DrawPoint(5, 5, false);
  c.DrawPointLine(0, 0, 20, 10);
  c.DrawBlockCircle(20, 10, 5);

  Element image = canvas(c);
  Element pet = vbox({
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

  Element gitStats = vflow({
      text(format("Current Branch: {}", branch.shortname())),
      text(format("Total Commit: {}", commitCount)),
      text(format("Last Commit: {}", head.sorthash())),
  });
  Element card = vflow({center(pet) | flex, filler(), gitStats | flex});

  return card;
}
