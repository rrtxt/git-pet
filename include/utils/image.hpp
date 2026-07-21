#include <cstdint>
#include <filesystem>
#include <vector>

struct RGBA {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

class Image {
public:
  static Image Load(const std::filesystem::path &path);

  int width() const;
  int height() const;

  const RGBA &at(int x, int y) const;

private:
  Image(int width, int height);

  int _width;
  int _height;
  int _channel;
  std::vector<RGBA> _pixels;
};
