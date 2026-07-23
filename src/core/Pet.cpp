#include "core/Pet.hpp"
#include <string>

std::string Pet::name() const { return _name; }

AnimationPlayer &Pet::animationPlayer() { return _animationPlayer; }

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
