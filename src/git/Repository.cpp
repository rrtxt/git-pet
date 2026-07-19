#include "git/Repository.hpp"
#include <filesystem>
#include <format>
#include <git2.h>
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
}

std::vector<Commit> Repository::history(size_t limit) const {
  git_revwalk *walker;

  git_revwalk_new(&walker, repo);
  git_revwalk_push_head(walker);

  std::vector<Commit> commits;

  git_oid oid;
  size_t counter = 0;

  while (git_revwalk_next(&oid, walker) == 0 && counter < limit) {
    git_commit *raw;
    git_commit_lookup(&raw, repo, &oid);

    Commit commit(raw);

    commits.emplace_back(commit);

    counter += 1;
  }

  git_revwalk_free(walker);

  return commits;
}

Branch Repository::currentBranch() const {
  git_reference *ref;
  if (git_repository_head(&ref, repo) != 0) {
    throw std::runtime_error("Cannot get current branch");
  }

  Branch branch(ref);

  return branch;
}

int Repository::commitCount() const { return 0; }

Repository::~Repository() { git_repository_free(repo); }
