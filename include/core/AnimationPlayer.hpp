#pragma once

#include "core/Animation.hpp"
#include <chrono>
#include <string>
#include <unordered_map>

class AnimationPlayer {
public:
  void update(std::chrono::milliseconds dt);

  void add(std::string name, Animation animation);

  void play(std::string name);
  void stop();

  const Image &currentFrame() const;

private:
  std::unordered_map<std::string, Animation> _animationMap;
  Animation *_currentAnimation = nullptr;
};
