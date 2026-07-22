#include <cstdint>
#include <stdexcept>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>

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

  img._image.assign(image, image + width * height * 4);
  img._channel = desired_channels;

  stbi_image_free(image);

  return img;
}

void Image::resize(int width, int height) {
  std::vector<uint8_t> resized_image(width * height * 4);
  stbir_resize_uint8_srgb(_image.data(), _width, _height, 0,
                          resized_image.data(), width, height, 0, STBIR_RGBA);

  _width = width;
  _height = height;
  _image = std::move(resized_image);
}

RGBA const Image::at(int x, int y) const {
  if (x < 0 || x >= _width || y < 0 || y >= _height) {
    throw std::out_of_range("Pixel out of range");
  }
  return RGBA{_image[((y * _width + x) * _channel) + 0],
              _image[((y * _width + x) * _channel) + 1],
              _image[((y * _width + x) * _channel) + 2],
              _image[((y * _width + x) * _channel) + 3]};
}

int Image::width() const { return _width; }
int Image::height() const { return _height; }
