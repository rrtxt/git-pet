
#include <git2/global.h>
class GitLibrary {
public:
  GitLibrary() { git_libgit2_init(); }

  ~GitLibrary() { git_libgit2_shutdown(); }
};
