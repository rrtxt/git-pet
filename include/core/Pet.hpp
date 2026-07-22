#pragma once
#include "Image.hpp"
#include <filesystem>
#include <string>

enum class PetStage { Egg, Baby, Teen, Adult };

class Pet {
public:
  Pet(std::string name, const std::filesystem::path &path,
      PetStage stage = PetStage::Egg)
      : _name(std::move(name)), _stage(stage), _image(Image::Load(path)) {};

  std::string name() const;
  std::string stage() const;
  Image image() const;

private:
  std::string _name;
  PetStage _stage;
  Image _image;
};
