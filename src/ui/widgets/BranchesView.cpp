#include <screen/ui/widgets/BranchesView.hpp>
#include <format>
#include <vector>

using namespace ftxui;
using namespace std;

Element BranchesView(const Repository &repo) {
  std::vector<Branch> branches = repo.branches();
  Elements list_items;

  list_items.push_back(text("Branches") | bold | color(Color::Green));
  list_items.push_back(separator());

  for (const auto &branch : branches) {
    if (branch.isHead()) {
      list_items.push_back(hbox({
          text("* ") | color(Color::Green) | bold,
          text(branch.shortname()) | color(Color::Green) | bold,
      }));
    } else {
      list_items.push_back(hbox({
          text("  ") | dim,
          text(branch.shortname()) | dim,
      }));
    }
  }

  return vbox(std::move(list_items));
}
