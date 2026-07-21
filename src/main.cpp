#include "screen/ui/widgets/GitCard.hpp"
#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>
#include <git/GitLibrary.hpp>
#include <git/Repository.hpp>
#include <git2.h>
#include <git2/repository.h>
#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
using namespace ftxui;
using namespace std;

std::string exec(const std::string &cmd) {
  char buffer[128];
  std::string result;

  FILE *pipe = popen(cmd.c_str(), "r");
  if (!pipe)
    return "";

  while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
    result += buffer;
  }

  pclose(pipe);
  return result;
}

int main() {
  GitLibrary git;

  std::filesystem::path path = std::filesystem::current_path();
  Repository repo(path);

  std::vector<Commit> commits = repo.history();
  Branch branch = repo.currentBranch();

  Pet pet("Milo");

  filesystem::path pet_path("assets/egg.png");
  filesystem::path combined_path = path / pet_path;
  Image image = Image::Load(combined_path);

  Element card = GitCard(pet, repo, image);

  auto screen = Screen::Create(Dimension::Fit(card), Dimension::Fit(card));

  Render(screen, card);
  screen.Print();

  return 0;
}
