#pragma once

#include "git/Repository.hpp"
#include <string>
#include <vector>
#include <filesystem>

struct AnimationConfig {
  std::string name;
  int fps = 0;
  std::filesystem::path path;
};

class LocalConfig {
public:
  static LocalConfig Load(const Repository &repo);
  void Save(const Repository &repo) const;

  LocalConfig(std::string id, std::string display_name)
      : _id(std::move(id)), _display_name(std::move(display_name)) {}

  const std::string &id() const { return _id; }
  const std::string &displayName() const { return _display_name; }

  void setId(std::string id) { _id = std::move(id); }
  void setDisplayName(std::string name) { _display_name = std::move(name); }

private:
  std::string _id;
  std::string _display_name;
};

class PetConfig {
public:
  static PetConfig Load(const std::string &pet_id);

  PetConfig(std::string id, std::string display_name,
            std::vector<std::string> stages,
            std::vector<AnimationConfig> animation_configs)
      : _id(std::move(id)),
        _display_name(std::move(display_name)),
        _stages(std::move(stages)),
        _animation_configs(std::move(animation_configs)) {}

  const std::string &id() const { return _id; }
  const std::string &displayName() const { return _display_name; }
  const std::vector<std::string> &stages() const { return _stages; }
  const std::vector<AnimationConfig> &animationConfigs() const { return _animation_configs; }

private:
  std::string _id;
  std::string _display_name;
  std::vector<std::string> _stages;
  std::vector<AnimationConfig> _animation_configs;
};
