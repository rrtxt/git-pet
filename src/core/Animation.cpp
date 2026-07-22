#include "core/Image.hpp"
#include <chrono>
#include <core/Animation.hpp>
#include <filesystem>
#include <vector>

Animation Animation::Load(const std::filesystem::path &path) {
  std::vector<std::filesystem::path> paths;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      paths.push_back(entry);
    }
  }

  std::sort(paths.begin(), paths.end());

  std::vector<Image> frames;
  for (const auto &path : paths) {
    Image frame = Image::Load(path);
    frames.push_back(frame);
  }

  Animation animation(frames);

  return animation;
}

void Animation::update(std::chrono::milliseconds dt) {
  if (!isPlaying) {
    return;
  }

  elapsed += dt;

  while (elapsed >= frameTime) {
    elapsed -= frameTime;
    currentFrameIndex = (currentFrameIndex + 1) % _frames.size();
  }
}

const Image &Animation::currentFrame() const {
  return _frames[currentFrameIndex];
}

void Animation::play() { isPlaying = true; }
void Animation::stop() { isPlaying = false; }
