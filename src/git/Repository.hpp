#pragma once

#include <git2.h>
#include <git2/types.h>
#include <string>

class Repository {
public:
  Repository(std::string path);
  int commitCount();
  ~Repository();

private:
  git_repository *repo_;
};
