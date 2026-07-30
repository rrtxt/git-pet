#include "core/AnimationPlayer.hpp"
#include "core/Animation.hpp"
#include <chrono>
#include <string>

void AnimationPlayer::update(std::chrono::milliseconds dt) {
  if (!_currentAnimationName.empty()) {
    auto it = _animationMap.find(_currentAnimationName);
    if (it != _animationMap.end()) {
      it->second.update(dt);
    }
  }
}

void AnimationPlayer::add(std::string name, Animation animation) {
  _animationMap.emplace(std::move(name), std::move(animation));
}

void AnimationPlayer::play(std::string name) {
  auto it = _animationMap.find(name);
  if (it == _animationMap.end()) {
    return;
  }

  if (!_currentAnimationName.empty()) {
    auto old_it = _animationMap.find(_currentAnimationName);
    if (old_it != _animationMap.end()) {
      old_it->second.stop();
    }
  }
  _currentAnimationName = name;
  it->second.play();
}

void AnimationPlayer::stop() {
  if (!_currentAnimationName.empty()) {
    auto it = _animationMap.find(_currentAnimationName);
    if (it != _animationMap.end()) {
      it->second.stop();
    }
  }
}

const Image &AnimationPlayer::currentFrame() const {
  if (_currentAnimationName.empty()) {
    static const Image empty_image;
    return empty_image;
  }
  auto it = _animationMap.find(_currentAnimationName);
  if (it == _animationMap.end()) {
    static const Image empty_image;
    return empty_image;
  }
  return it->second.currentFrame();
}
