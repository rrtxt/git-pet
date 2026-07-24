#include "git/Repository.hpp"
#include "git/Branch.hpp"
#include <filesystem>
#include <format>
#include <git2.h>
#include <git2/branch.h>
#include <git2/commit.h>
#include <git2/refs.h>
#include <git2/repository.h>
#include <git2/types.h>
#include <stdexcept>
#include <vector>

Repository::Repository(const std::filesystem::path &path) {
  if (git_repository_open(
          &repo, std::filesystem::absolute(path).string().c_str()) != 0)
    throw std::runtime_error(
        std::format("Cannot open repository, repository {} not found\n",
                    path.relative_path().string()));

  const char *workdir = git_repository_workdir(repo);
  if (workdir) {
    std::filesystem::path p(workdir);
    _name = p.parent_path().filename().string();
  } else {
    _name = path.filename().string();
  }
}


std::string Repository::name() const { return _name; }

std::vector<Commit> Repository::history(size_t limit) const {
  git_revwalk *walker = nullptr;

  if (git_revwalk_new(&walker, repo) != 0) {
    return {};
  }
  if (git_revwalk_push_head(walker) != 0) {
    git_revwalk_free(walker);
    return {};
  }

  std::vector<Commit> commits;

  git_oid oid;
  size_t counter = 0;

  while (git_revwalk_next(&oid, walker) == 0 && counter < limit) {
    git_commit *raw = nullptr;
    if (git_commit_lookup(&raw, repo, &oid) != 0)
      continue;

    Commit commit(raw);
    git_commit_free(raw);

    commits.emplace_back(commit);

    counter += 1;
  }

  git_revwalk_free(walker);

  return commits;
}

Branch Repository::currentBranch() const {
  git_reference *ref = nullptr;
  if (git_repository_head(&ref, repo) != 0) {
    throw std::runtime_error("Cannot get current branch");
  }

  Branch branch(ref);
  git_reference_free(ref);

  return branch;
}

std::vector<Branch> Repository::branches() const {
  git_branch_iterator *iter = nullptr;
  if (git_branch_iterator_new(&iter, repo, GIT_BRANCH_ALL) != 0) {
    throw std::runtime_error("Cannot list all branch");
  }

  std::vector<Branch> branches;
  git_reference *branch_ref = nullptr;
  git_branch_t branch_type;

  while (git_branch_next(&branch_ref, &branch_type, iter) == 0) {
    if (git_reference_is_branch(branch_ref) != 0) {
      Branch branch(branch_ref);
      branches.push_back(branch);
    }
    git_reference_free(branch_ref);
  }

  git_branch_iterator_free(iter);

  return branches;
}

int Repository::commitCount() const {
  git_revwalk *walker = nullptr;
  if (git_revwalk_new(&walker, repo) != 0) {
    return 0;
  }
  if (git_revwalk_push_head(walker) != 0) {
    git_revwalk_free(walker);
    return 0;
  }

  git_oid oid;
  int count = 0;

  while (git_revwalk_next(&oid, walker) == 0) {
    ++count;
  }

  git_revwalk_free(walker);

  return count;
}

int Repository::commitCountByTime(const TimeRange &range) const {
  git_revwalk *walker = nullptr;
  if (git_revwalk_new(&walker, repo) != 0) {
    return 0;
  }
  if (git_revwalk_push_head(walker) != 0) {
    git_revwalk_free(walker);
    return 0;
  }

  git_oid oid;
  int count = 0;

  while (git_revwalk_next(&oid, walker) == 0) {
    git_commit *commit = nullptr;
    if (git_commit_lookup(&commit, repo, &oid) != 0)
      continue;

    git_time_t time = git_commit_time(commit);

    // When commit date outside current time range, break the loop
    if (time >= range.start && time <= range.end) {
      ++count;
    }
    git_commit_free(commit);
  }

  git_revwalk_free(walker);

  return count;
}

int Repository::commitCountPerWeek() const {
  return commitCountByTime(getCurrentWeek());
}

int Repository::commitCountPerDay() const {
  return commitCountByTime(getCurrentDay());
}

Commit Repository::head() const {
  git_reference *head = nullptr;
  if (git_repository_head(&head, repo) != 0) {
    return Commit();
  }

  const git_oid *oid = git_reference_target(head);
  if (!oid) {
    git_reference_free(head);
    return Commit();
  }

  git_commit *commit = nullptr;
  if (git_commit_lookup(&commit, repo, oid) != 0) {
    git_reference_free(head);
    return Commit();
  }

  Commit new_commit(commit);

  git_commit_free(commit);
  git_reference_free(head);

  return new_commit;
}

Repository::~Repository() { git_repository_free(repo); }
