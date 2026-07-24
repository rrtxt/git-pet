#pragma once
#include <ctime>
#include <git2/types.h>
#include <string>

class Commit {
public:
  Commit() : _hash("0000000000000000000000000000000000000000"), _message("No commits yet"), _author("None"), _date(0) {}
  Commit(git_commit *commit);
  std::string hash() const;

  std::string sorthash() const;
  std::string message() const;
  std::string author() const;
  std::time_t date() const;

private:
  std::string _hash;
  std::string _message;
  std::string _author;
  std::time_t _date;
};
