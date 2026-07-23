#include "core/AnimationPlayer.hpp"
#include "core/Animation.hpp"
#include <chrono>
#include <string>

void AnimationPlayer::update(std::chrono::milliseconds dt) {
  if (_currentAnimation)
    _currentAnimation->update(dt);
}

void AnimationPlayer::add(std::string name, Animation &animation) {
  _animationMap.emplace(name, animation);
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

void AnimationPlayer::stop() { _currentAnimation->stop(); }

const Image &AnimationPlayer::currentFrame() const {
  return _currentAnimation->currentFrame();
}
