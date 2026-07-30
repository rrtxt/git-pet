#include "core/Image.hpp"
#include <chrono>
#include <core/Animation.hpp>
#include <filesystem>
#include <vector>
#include <iostream>

Animation Animation::Load(const std::filesystem::path &path) {
  std::vector<std::filesystem::path> paths;
  if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
      if (entry.is_regular_file()) {
        paths.push_back(entry);
      }
    }
  }

  std::sort(paths.begin(), paths.end());

  std::vector<Image> frames;
  for (const auto &p : paths) {
    try {
      Image frame = Image::Load(p);
      frames.push_back(frame);
    } catch (const std::exception &e) {
      std::cerr << "Warning: Failed to load animation frame from " << p.string()
                << " - " << e.what() << std::endl;
    }
  }

  Animation animation(frames);

  return animation;
}

void Animation::update(std::chrono::milliseconds dt) {
  if (!isPlaying || _frames.empty()) {
    return;
  }

  elapsed += dt;

  while (elapsed >= frameTime) {
    elapsed -= frameTime;
    currentFrameIndex = (currentFrameIndex + 1) % _frames.size();
  }
}

const Image &Animation::currentFrame() const {
  if (_frames.empty()) {
    static const Image empty_image;
    return empty_image;
  }
  return _frames[currentFrameIndex];
}

void Animation::play() { isPlaying = true; }
void Animation::stop() { isPlaying = false; }
