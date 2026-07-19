#include <git2/types.h>
#include <string>

class Branch {
public:
  Branch(git_reference *ref);
  std::string name() const;
  std::string shortname() const;
  bool isHead() const;

private:
  std::string _name;
  std::string _shortname;
  bool _is_head;
};
