#include <screen/ui/widgets/MainView.hpp>
#include <format>

using namespace ftxui;
using namespace std;

Element MainView(const Repository &repo) {
  Commit head = repo.head();
  return vbox({
      text("Git Statistics") | bold | color(Color::Green),
      separator(),
      text(format("Today's Commit: {}", repo.commitCountPerDay())),
      text(format("This week: {}", repo.commitCountPerWeek())),
      text(format("Total Commit: {}", repo.commitCount())),
      text(format("Last Commit: {}", head.sorthash())),
  });
}
