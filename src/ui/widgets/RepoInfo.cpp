#include <screen/ui/widgets/RepoInfo.hpp>
#include <format>

using namespace ftxui;
using namespace std;

Element RepoInfo(const Repository &repo) {
  Branch branch = repo.currentBranch();
  return vflow({
      text(format("Repository: {}", repo.name())),
      text(format("Branch: {}", branch.shortname())),
  });
}
