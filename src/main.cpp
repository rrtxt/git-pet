#include <cpptui.hpp>
#include <filesystem>
#include <git/GitLibrary.hpp>
#include <git/Repository.hpp>
#include <git2.h>
#include <git2/repository.h>
#include <string>
#include <vector>

using namespace cpptui;

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
  Repository repo = Repository(path);

  std::vector<Commit> commits = repo.history();

  for (Commit commit : commits) {
    std::cout << "commit " << commit.hash() << std::endl;
    std::cout << "Author: " << commit.author() << std::endl;
    std::cout << "Date': " << commit.date() << std::endl;
    std::cout << "Message: \n" << commit.message() << std::endl;
  }

  return 0;
}
