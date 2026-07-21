#include <screen/ui/widgets/RepoInfo.hpp>
#include <format>

using namespace ftxui;
using namespace std;

Element RepoInfo(const Repository &repo) {
  Branch branch = repo.currentBranch();
  return vbox({
      text(format("Repository: {}", repo.name())) | bold,
      text(format("Branch: {}", branch.shortname())) | dim,
  });
}
