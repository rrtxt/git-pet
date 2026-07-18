#include "Repository.hpp"
#include <git2.h>
#include <git2/repository.h>
#include <stdexcept>
/*
class Repository {
public:
  Repository(const std::string &path) {
    if (git_repository_open(&repo_, path.c_str()) != 0)
      throw std::runtime_error("Cannot open repository");
  }

  int countCommit() { return 0; }

  ~Repository() { git_repository_free(repo_); }

private:
  git_repository *repo_;
}; */

Repository::Repository(std::string path) {
  if (git_repository_open(&repo_, path.c_str()) != 0)
    throw std::runtime_error("Cannot open repository, repository %s not found");
}

int Repository::commitCount() { return 0; }

Repository::~Repository() { git_repository_free(repo_); }
