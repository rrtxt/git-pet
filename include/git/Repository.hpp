#pragma once

#include <filesystem>
#include <git2.h>
#include <git2/types.h>

#include <git/Branch.hpp>
#include <git/Commit.hpp>
#include <vector>

class Repository {
public:
  explicit Repository(const std::filesystem::path &path);

  Branch currentBranch() const;

  int commitCount() const;

  Commit head() const;

  std::vector<Commit> history(size_t limit = 20) const;

  bool isDirty() const;

  ~Repository();

private:
  git_repository *repo;
};
