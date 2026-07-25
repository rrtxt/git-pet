#include "core/Config.hpp"
#include "git/Repository.hpp"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <toml.hpp>

// ==========================================
// LocalConfig Implementation
// ==========================================

LocalConfig LocalConfig::Load(const Repository &repo) {
  std::filesystem::path config_path =
      std::filesystem::path(repo.directory()) / "pet.config";

  std::string id = "cat";
  std::string display_name = repo.name();

  if (std::filesystem::exists(config_path)) {
    std::ifstream config_file(config_path);
    if (!config_file.is_open()) {
      std::cerr << "Could not open the file " << config_path << std::endl;
    }

    std::string line;
    std::vector<std::string> tokens;

    while (std::getline(config_file, line)) {
      std::istringstream lineStream(line);
      std::string str;

      while (std::getline(lineStream, str, '=')) {
        str.erase(std::remove_if(str.begin(), str.end(),
                                 [](unsigned char ch) {
                                   return std::isspace(ch) || !std::isalnum(ch);
                                 }),
                  str.end());
        tokens.push_back(str);
      }
    }

    for (size_t i = 0; i < tokens.size(); i += 2) {
      if (tokens[i] == "Name" || tokens[i] == "name") {
        display_name = tokens[i + 1];
      } else if (tokens[i] == "id" || tokens[i] == "Id") {
        id = tokens[i + 1];
      }
    }
  } else {
    std::ofstream config(config_path);
    if (!config.is_open()) {
      std::cerr << "Could not create new config file" << std::endl;
    }

    config << "id" << "=" << id << std::endl;
    config << "name" << "=" << display_name << std::endl;
    config.close();
  }

  return LocalConfig(std::move(id), std::move(display_name));
}

void LocalConfig::Save(const Repository &repo) const {
  std::filesystem::path config_path =
      std::filesystem::path(repo.directory()) / "pet.config";
  std::ofstream config(config_path);

  if (!config.is_open()) {
    std::cerr << "Could not open config file for writing" << std::endl;
    return;
  }

  config << "id=" << _id << std::endl;
  config << "name=" << _display_name << std::endl;
}

// ==========================================
// PetConfig Implementation
// ==========================================

PetConfig PetConfig::Load(const std::string &pet_id) {
  const char* path_env = std::getenv("GIT_PET_PATH");
  std::string path = path_env ? path_env : "";
  std::filesystem::path pet_path = std::filesystem::path(path) / pet_id;
  std::filesystem::path pet_config_path = pet_path / "pet.toml";

  if (!std::filesystem::exists(pet_config_path)) {
    throw std::runtime_error("Could not find pet.toml in: " + pet_config_path.string());
  }

  std::vector<std::string> stageNames;
  std::vector<AnimationConfig> animationConfigs;

  auto config = toml::parse_file(pet_config_path.string());

  auto *stages = config["stages"].as_array();
  if (!stages)
    throw std::runtime_error("Missing 'stages' array");

  for (const auto &stage_node : *stages) {
    auto stage_name = stage_node.value<std::string>();
    if (!stage_name)
      continue;

    stageNames.push_back(*stage_name);

    auto *table = config[*stage_name].as_table();
    if (!table)
      continue;

    int fps = static_cast<int>(table->get_as<int64_t>("fps")->get());

    animationConfigs.emplace_back(*stage_name, fps, pet_path / *stage_name);
  }

  std::string display_name = pet_id;
  if (auto name_val = config["name"].value<std::string>()) {
    display_name = *name_val;
  }

  return PetConfig(pet_id, std::move(display_name), std::move(stageNames), std::move(animationConfigs));
}
