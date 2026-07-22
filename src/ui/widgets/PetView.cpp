#include <format>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <screen/ui/widgets/PetView.hpp>

using namespace ftxui;
using namespace std;

namespace {
Element Pixel(Color color) { return text("  ") | bgcolor(color); }

Element RenderImage(const Image &image) {
  Elements rows;

  for (int y = 0; y < image.height(); y++) {
    Elements cols;
    for (int x = 0; x < image.width(); x++) {
      auto p = image.at(x, y);

      cols.push_back(Pixel(Color::RGBA(p.r, p.g, p.b, p.a)));
    }

    rows.push_back(hbox(std::move(cols)));
  }

  return vbox(std::move(rows));
}
} // namespace

Element PetView(Pet &pet, const Image &petImage) {
  Element image;
  if (petImage.width() > 0 && petImage.height() > 0) {
    image = RenderImage(petImage);
  } else {
    // Fallback pixel art (pet_element)
    image = vbox({
        hbox({
            Pixel(Color::Black),
            Pixel(Color::Red),
            Pixel(Color::Red),
            Pixel(Color::Black),
        }),
        hbox({
            Pixel(Color::Red),
            Pixel(Color::White),
            Pixel(Color::White),
            Pixel(Color::Red),
        }),
        hbox({
            Pixel(Color::Black),
            Pixel(Color::Red),
            Pixel(Color::Red),
            Pixel(Color::Black),
        }),
    });
  }

  Element petStats = vbox({
      text(format("Pet: {}", pet.name())) | bold,
      text(format("Stage: {}", pet.stage())) | dim,
  });

  return vbox({
      filler() | flex,
      center(image) | flex,
      filler() | flex,
      petStats | hcenter,
  });
}
