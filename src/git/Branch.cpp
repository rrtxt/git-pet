#include "git/Repository.hpp"
#include <git2/branch.h>
#include <git2/refs.h>
#include <git2/types.h>
#include <stdexcept>

Branch::Branch(git_reference *ref) {
  if (git_reference_is_branch(ref) == 0) {
    throw std::runtime_error("Reference is not branch");
  }
  const char *shortname;
  git_branch_name(&shortname, ref);

  _name = git_reference_name(ref);
  _shortname = shortname;
  _is_head = git_branch_is_head(ref);

  _ref = ref;
}

std::string Branch::name() const { return _name; };
std::string Branch::shortname() const { return _shortname; };
bool Branch::isHead() const { return _is_head; };

Branch::~Branch() { git_reference_free(_ref); }
