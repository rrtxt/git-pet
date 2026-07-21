#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <utils/image.hpp>

Image::Image(int width, int height) {
  _width = width;
  _height = height;
}

Image Image::Load(const std::filesystem::path &path) {
  int width, height, channels;
  constexpr int desired_channels = STBI_rgb_alpha;
  unsigned char *image = stbi_load(path.string().c_str(), &width, &height,
                                   &channels, desired_channels);

  if (!image) {
    throw std::runtime_error("Image is not found");
  }

  Image img(width, height);

  img._channel = desired_channels;

  img._pixels.resize(width * height);
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      int index = (y * width + x) * desired_channels;
      RGBA pixel = {image[index + 0], image[index + 1], image[index + 2],
                    image[index + 3]};
      // img._pixels.push_back(pixel);
      img._pixels[y * width + x] = pixel;
    }
  }
  stbi_image_free(image);

  return img;
}

RGBA const &Image::at(int x, int y) const {
  if (x < 0 || x >= _width || y < 0 || y >= _height) {
    throw std::out_of_range("Pixel out of range");
  }
  return _pixels[y * _width + x];
}

int Image::width() const { return _width; }
int Image::height() const { return _height; }
