#include "core/Pet.hpp"
#include <string>

Pet::Pet(std::string name, PetStage stage) {
  _name = name;
  _stage = stage;
}

std::string Pet::name() const { return _name; }

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
  }
}
