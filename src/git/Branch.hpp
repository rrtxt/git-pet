#include <string>

class Branch {
public:
  std::string name() const;
  bool isHead() const;

private:
  std::string _name;
  bool _is_head;
};
