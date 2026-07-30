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
  Repository(const Repository &) = delete;
  Repository &operator=(const Repository &) = delete;

  std::string name() const;

  Branch currentBranch() const;
  std::vector<Branch> branches() const;

  int commitCount() const;
  int commitCountByTime(const TimeRange &range) const;

  int commitCountPerWeek() const;
  int commitCountPerDay() const;

  Commit head() const;

  std::vector<Commit> history(size_t limit = 20) const;
  std::string directory() const;

  void status() const;

  bool isDirty() const;
  bool hasConflicts() const;

  ~Repository();

private:
  git_repository *repo;
  std::string _name;
};
