#pragma once

#include "git/Repository.hpp"
#include <string>
#include <vector>

struct AnimationConfig {
  std::string name;
  int fps = 0;
  std::filesystem::path path;
};

struct PetConfig {
  std::string id;
  std::string display_name;
  std::vector<std::string> stages;
  std::vector<AnimationConfig> animation_configs;
};

struct LocalConfig {
  std::string id;
  std::string display_name;
};

class Config {
public:
  static Config Load(Repository &repo);

  Config(LocalConfig localConfig, PetConfig petConfig)
      : local_config(localConfig), pet_config(petConfig) {};
  const LocalConfig localConfig();
  const PetConfig petConfig();

  void writeLocal(std::filesystem::path &path);

private:
  LocalConfig local_config;
  PetConfig pet_config;
};
