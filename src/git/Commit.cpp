
#include "git/Commit.hpp"
#include <git2/commit.h>
#include <git2/oid.h>

Commit::Commit(git_commit *commit) {
  const git_oid *oid = git_commit_id(commit);

  // generate git normal hash size (40)
  char sha[40] = {0};
  git_oid_tostr(sha, 39, oid);

  _hash = sha;
  _message = git_commit_message(commit);
  _author = git_commit_author(commit)->email;
  _date = git_commit_time(commit);

  git_commit_free(commit);
}

std::string Commit::hash() const { return _hash; }
std::string Commit::sorthash() const { return _hash.substr(0, 10); }
std::string Commit::message() const { return _message; }
std::string Commit::author() const { return _author; }
std::time_t Commit::date() const { return _date; }
