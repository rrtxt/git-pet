#include "Repository.hpp"
#include <filesystem>
#include <format>
#include <git2.h>
#include <git2/repository.h>
#include <stdexcept>

Repository::Repository(const std::filesystem::path &path) {
  if (git_repository_open(
          &repo, std::filesystem::absolute(path).string().c_str()) != 0)
    throw std::runtime_error(
        std::format("Cannot open repository, repository {} not found\n",
                    path.relative_path().string()));
}

Branch Repository::currentBranch() const {}

int Repository::commitCount() const { return 0; }

Repository::~Repository() { git_repository_free(repo); }
