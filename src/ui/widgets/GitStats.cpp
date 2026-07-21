#include <screen/ui/widgets/GitStats.hpp>
#include <format>

using namespace ftxui;
using namespace std;

Element GitStats(const Repository &repo) {
  Commit head = repo.head();
  return vflow({
      text(format("Today's Commit: {}", repo.commitCountPerDay())),
      text(format("This week: {}", repo.commitCountPerWeek())),
      text(format("Total Commit: {}", repo.commitCount())),
      text(format("Last Commit: {}", head.sorthash())),
  });
}
