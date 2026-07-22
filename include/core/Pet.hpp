#pragma once
#include "Image.hpp"
#include "core/Animation.hpp"
#include <chrono>
#include <filesystem>
#include <string>

enum class PetStage { Egg, Baby, Teen, Adult };

class Pet {
public:
  Pet(std::string name, const std::filesystem::path &path,
      PetStage stage = PetStage::Egg)
      : _name(std::move(name)), _stage(stage),
        _currentAnimation(Animation::Load(path)) {};

  void update(std::chrono::milliseconds dt);

  std::string name() const;
  std::string stage() const;
  const Image &image() const;
  Animation &animation();

private:
  std::string _name;
  PetStage _stage;
  Animation _currentAnimation;
};
