#include <screen/ui/widgets/GitCard.hpp>
#include <screen/ui/widgets/PetView.hpp>
#include <screen/ui/widgets/RepoInfo.hpp>
#include <screen/ui/widgets/GitStats.hpp>

using namespace ftxui;

Element GitCard(Pet &pet, const Repository &repo, const Image &petImage) {
  Element left_column = PetView(pet, petImage);
  Element right_column = vbox({
      RepoInfo(repo),
      separator(),
      filler() | flex,
      GitStats(repo),
  });

  Element card =
      hbox({
          left_column | flex,
          separator(),
          right_column | flex,
      }) | border;

  return card;
}
