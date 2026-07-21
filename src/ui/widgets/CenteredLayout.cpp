#include <screen/ui/widgets/CenteredLayout.hpp>
#include <utility>

using namespace ftxui;

Element CenteredLayout(Element content) {
  return vbox({
      filler(),
      hbox({
          filler(),
          std::move(content),
          filler(),
      }),
      filler(),
  });
}
