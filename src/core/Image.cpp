#include <cstdint>
#include <stdexcept>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>

#include <core/Image.hpp>

Image::Image(int width, int height, int channel) {
  _width = width;
  _height = height;
  _channel = channel;

  _image = std::vector<uint8_t>(width * height * channel);
}

Image Image::Load(const std::filesystem::path &path) {
  int width, height, channels;
  constexpr int desired_channels = STBI_rgb_alpha;
  unsigned char *image = stbi_load(path.string().c_str(), &width, &height,
                                   &channels, desired_channels);

  if (!image) {
    throw std::runtime_error("Image is not found");
  }

  Image img(width, height, desired_channels);

  img._image.assign(image, image + width * height * 4);

  stbi_image_free(image);

  return img;
}

Image Image::Resized(int width, int height) {
  Image output(width, height, _channel);

  stbir_resize_uint8_srgb(_image.data(), _width, _height, 0,
                          output._image.data(), width, height, 0, STBIR_RGBA);

  return output;
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
