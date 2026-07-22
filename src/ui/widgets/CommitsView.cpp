#include <screen/ui/widgets/CommitsView.hpp>
#include <format>
#include <vector>

using namespace ftxui;
using namespace std;

Element CommitsView(const Repository &repo) {
  std::vector<Commit> commits = repo.history(8);
  Elements list_items;

  list_items.push_back(text("Latest Commits") | bold | color(Color::Cyan));
  list_items.push_back(separator());

  for (const auto &commit : commits) {
    // Truncate commit message to keep it neat inside the card
    std::string msg = commit.message();
    // Remove trailing newlines from message if any
    while (!msg.empty() && (msg.back() == '\n' || msg.back() == '\r')) {
      msg.pop_back();
    }
    if (msg.length() > 18) {
      msg = msg.substr(0, 15) + "...";
    }

    list_items.push_back(hbox({
        text(commit.sorthash()) | color(Color::Yellow) | bold,
        text(" "),
        text(msg),
        filler(),
        text("(" + commit.author() + ")") | dim,
    }));
  }

  return vbox(std::move(list_items));
}
