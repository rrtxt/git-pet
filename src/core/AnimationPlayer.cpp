#include "core/AnimationPlayer.hpp"
#include "core/Animation.hpp"
#include <chrono>
#include <string>

void AnimationPlayer::update(std::chrono::milliseconds dt) {
  if (_currentAnimation)
    _currentAnimation->update(dt);
}

void AnimationPlayer::add(std::string name, Animation animation) {
  _animationMap.emplace(std::move(name), std::move(animation));
}

void AnimationPlayer::play(std::string name) {
  auto it = _animationMap.find(name);
  if (it == _animationMap.end()) {
    return;
  }

  if (_currentAnimation) {
    _currentAnimation->stop();
  }
  _currentAnimation = &it->second;

  _currentAnimation->play();
}

void AnimationPlayer::stop() {
  if (_currentAnimation)
    _currentAnimation->stop();
}

const Image &AnimationPlayer::currentFrame() const {
  if (!_currentAnimation) {
    static const Image empty_image;
    return empty_image;
  }
  return _currentAnimation->currentFrame();
}
