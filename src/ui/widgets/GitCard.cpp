#include <screen/ui/widgets/GitCard.hpp>
#include <screen/ui/widgets/PetView.hpp>
#include <screen/ui/widgets/RepoInfo.hpp>
#include <screen/ui/widgets/GitStats.hpp>

using namespace ftxui;

Element GitCard(Pet &pet, const Repository &repo, const Image &petImage) {
  Element card =
      vflow({
          filler() | flex,
          PetView(pet, petImage) | flex,
          filler() | flex,
          RepoInfo(repo),
          filler() | flex,
          GitStats(repo),
      }) | border;

  return card;
}
