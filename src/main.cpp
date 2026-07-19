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
  cout << "git" << endl;

  std::filesystem::path path = std::filesystem::current_path();
  Repository repo(path);

  cout << "repo" << endl;
  std::vector<Commit> commits = repo.history();

  cout << "commit" << endl;
  Branch branch = repo.currentBranch();

  cout << "branch" << endl;
  /* std::cout << "Current Branch: " << branch.shortname() << std::endl;
  std::cout << "Total Commit: " << commits.size() << std::endl;
  std::cout << "Last Commit: " << commits[0].sorthash() << std::endl; */
  // std::cout << "Is HEAD: " << branch.isHead() << std::endl;
  /* for (Commit commit : commits) {
    std::cout << "commit " << commit.hash() << std::endl;
    std::cout << "Author: " << commit.author() << std::endl;
    std::cout << "Date': " << commit.date() << std::endl;
    std::cout << "Message: \n" << commit.message() << std::endl;
  } */

  Element card = GitCard(repo);
  cout << "card" << endl;
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(card));

  Render(screen, card);
  screen.Print();

  return 0;
}
