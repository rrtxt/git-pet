#pragma once
#include "core/AnimationPlayer.hpp"
#include <chrono>
#include <string>

enum class PetStage { Egg, Baby, Teen, Adult };
enum class PetMood { Sad, Happy, Angry, Neutral };

class LocalConfig;
class PetConfig;

class Pet {
public:
  static Pet Load(const LocalConfig &localConfig, const PetConfig &petConfig, PetStage stage, PetMood mood = PetMood::Neutral);

  Pet(std::string name, AnimationPlayer animationPlayer,
      PetStage stage = PetStage::Egg, PetMood mood = PetMood::Neutral)
      : _name(std::move(name)), _stage(stage), _mood(mood),
        _animationPlayer(std::move(animationPlayer)) {};

  void update(std::chrono::milliseconds dt);

  std::string name() const;
  std::string stage() const;
  std::string mood() const;
  PetMood moodEnum() const { return _mood; }
  void setMood(PetMood mood) { _mood = mood; }
  AnimationPlayer &animationPlayer();

private:
  std::string _name;
  PetStage _stage;
  PetMood _mood;
  AnimationPlayer _animationPlayer;
};
