#include "Repository.hpp"
#include <filesystem>
#include <format>
#include <git2.h>
#include <git2/repository.h>
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

  while (git_revwalk_next(&oid, walker) == 0) {
    git_commit *raw;
    git_commit_lookup(&raw, repo, &oid);

    Commit commit(raw);

    commits.emplace_back(commit);

    git_commit_free(raw);
  }

  git_revwalk_free(walker);

  return commits;
}

Branch Repository::currentBranch() const {}

int Repository::commitCount() const { return 0; }

Repository::~Repository() { git_repository_free(repo); }
