#pragma once
#include "core/AnimationPlayer.hpp"
#include <chrono>
#include <string>

enum class PetStage { Egg, Baby, Teen, Adult };

class LocalConfig;
class PetConfig;

class Pet {
public:
  static Pet Load(const LocalConfig &localConfig, const PetConfig &petConfig, PetStage stage);

  Pet(std::string name, AnimationPlayer animationPlayer,
      PetStage stage = PetStage::Egg)
      : _name(std::move(name)), _stage(stage),
        _animationPlayer(std::move(animationPlayer)) {};

  void update(std::chrono::milliseconds dt);

  std::string name() const;
  std::string stage() const;
  AnimationPlayer &animationPlayer();

private:
  std::string _name;
  PetStage _stage;
  AnimationPlayer _animationPlayer;
};
