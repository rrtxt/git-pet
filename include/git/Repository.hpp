#pragma once

#include <ctime>
#include <filesystem>
#include <git2.h>
#include <git2/types.h>

#include <git/Branch.hpp>
#include <git/Commit.hpp>
#include <utils/time.hpp>
#include <vector>

class Repository {
public:
  explicit Repository(const std::filesystem::path &path);

  std::string name() const;

  Branch currentBranch() const;

  int commitCount() const;
  int commitCountByTime(const TimeRange &range) const;

  int commitCountPerWeek() const;
  int commitCountPerDay() const;

  Commit head() const;

  std::vector<Commit> history(size_t limit = 20) const;

  bool isDirty() const;

  ~Repository();

private:
  git_repository *repo;
  std::string _name;
};
