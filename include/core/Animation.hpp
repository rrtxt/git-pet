#pragma once

#include "Image.hpp"
#include <chrono>
#include <vector>

class Animation {
public:
  static Animation Load(const std::filesystem::path &path);

  Animation(std::vector<Image> frames) : _frames(frames) {};

  void update(std::chrono::milliseconds dt);
  const Image &currentFrame() const;

  void play();
  void stop();

private:
  std::vector<Image> _frames;
  int currentFrameIndex = 0;
  bool isPlaying = false;

  std::chrono::milliseconds frameTime{150};
  std::chrono::milliseconds elapsed{0};
};
