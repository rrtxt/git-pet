#include <format>
#include <ftxui/dom/elements.hpp>
#include <screen/ui/widgets/GitCard.hpp>

using namespace ftxui;
using namespace std;

Element GitCard(const Repository &repo) {
  Branch branch = repo.currentBranch();
  int commitCount = repo.commitCount();
  Commit head = repo.head();

  Element card = vbox({
      text(format("Current Branch: {}", branch.name())) | border,
      text(format("Total Commit: {}", commitCount)) | border,
      text(format("Last Commit: {}", head.sorthash())) | border,
  });

  return card;
}
