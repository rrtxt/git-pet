#pragma once
#include <string>

enum class PetStage { Egg, Baby, Teen, Adult };

class Pet {
public:
  Pet(std::string name, PetStage stage = PetStage::Egg);
  std::string name() const;
  std::string stage() const;

private:
  std::string _name;
  PetStage _stage;
};
