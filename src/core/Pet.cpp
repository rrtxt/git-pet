#include "core/Pet.hpp"
#include "core/Config.hpp"
#include "core/Animation.hpp"
#include <algorithm>
#include <cctype>
#include <string>

namespace {
std::string toLower(std::string str) {
  std::transform(str.begin(), str.end(), str.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return str;
}
} // namespace

Pet Pet::Load(const LocalConfig &localConfig, const PetConfig &petConfig, PetStage stage, PetMood mood) {
  AnimationPlayer animationPlayer;

  for (const auto &animConfig : petConfig.animationConfigs()) {
    Animation anim = Animation::Load(animConfig.path);
    if (animConfig.fps > 0) {
      anim.setFrameTime(std::chrono::milliseconds(1000 / animConfig.fps));
    }
    animationPlayer.add(animConfig.name, std::move(anim));
  }

  Pet pet(localConfig.displayName(), std::move(animationPlayer), stage, mood);
  pet.animationPlayer().play(toLower(pet.stage()));

  return pet;
}

std::string Pet::mood() const {
  switch (_mood) {
  case PetMood::Sad:
    return "Sad";
  case PetMood::Happy:
    return "Happy";
  case PetMood::Angry:
    return "Angry";
  case PetMood::Neutral:
    return "Neutral";
  default:
    return "Unknown";
  }
}

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
