#include "core/Pet.hpp"
#include "core/Animation.hpp"
#include <chrono>
#include <string>

std::string Pet::name() const { return _name; }

const Image &Pet::image() const { return _currentAnimation.currentFrame(); }

Animation &Pet::animation() { return _currentAnimation; }

void Pet::update(std::chrono::milliseconds dt) { _currentAnimation.update(dt); }

std::string Pet::stage() const {
  switch (_stage) {
  case PetStage::Egg:
    return "Egg";
  case PetStage::Baby:
    return "Baby";
  case PetStage::Teen:
    return "Teen";
  case PetStage::Adult:
    return "Adult";
  default:
    return "Unknown";
  }
}
