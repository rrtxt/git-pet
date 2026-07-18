#include <cpptui.hpp>
#include <memory>
#include <string>

using namespace cpptui;

int main() {
  App app;

  Theme::set_theme(Theme::Dark());
  auto root = std::make_shared<Align>();

  auto main_layout = std::make_shared<Border>(BorderStyle::Rounded);
  auto main_vertical = std::make_shared<Vertical>();

  auto label = std::make_shared<Label>("Git-Pet");

  auto content_container = std::make_shared<Horizontal>();
  auto pet_container = std::make_shared<Border>(BorderStyle::Rounded);
  auto pet_info_container = std::make_shared<Border>(BorderStyle::Rounded);

  root->add(main_layout);
  main_layout->add(main_vertical);

  main_vertical->add(label);
  main_vertical->add(content_container);
  content_container->add(pet_container);
  content_container->add(pet_info_container);

  app.register_exit_key('q');
  app.run(root);

  return 0;
}
