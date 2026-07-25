#include "core/Config.hpp"
#include "git/Repository.hpp"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <git2/repository.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <toml.hpp>

Config Config::Load(Repository &repo) {
  std::filesystem::path config_path =
      std::filesystem::path(repo.directory()) / "pet.config";

  std::cout << config_path << std::endl;

  // Load Local config
  LocalConfig localConfig;
  if (std::filesystem::exists(config_path)) {
    std::ifstream config_file(config_path);

    if (!config_file.is_open()) {
      std::cerr << "Could not open the file" << std::endl;
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

    for (int i = 0; i < tokens.size(); i += 2) {
      if (tokens[i] == "Name" || tokens[i] == "name") {
        localConfig.display_name = tokens[i + 1];
      } else if (tokens[i] == "id" || tokens[i] == "Id") {
        localConfig.id = tokens[i + 1];
      } else {
        continue;
      }
    }
  } else {
    std::ofstream config(config_path);

    if (!config.is_open()) {
      std::cerr << "Could not create new config file" << std::endl;
    }

    config << "id" << "=" << "cat" << std::endl;
    config << "name" << "=" << repo.name() << std::endl;

    localConfig.id = "cat";
    localConfig.display_name = repo.name();

    config.close();
  }

  // return Config(localConfig);

  // Load Pet Config
  std::string path = std::getenv("GIT_PET_PATH");
  std::filesystem::path pet_path = std::filesystem::path(path) / localConfig.id;
  std::filesystem::path pet_config_path = pet_path / "pet.toml";

  if (!std::filesystem::exists(pet_config_path)) {
    std::cerr << "Could not find pet.toml in: " << pet_config_path << std::endl;
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

  PetConfig petConfig{localConfig.id, localConfig.display_name,
                      std::move(stageNames), std::move(animationConfigs)};

  return Config(localConfig, petConfig);
}

const LocalConfig Config::localConfig() { return local_config; }
const PetConfig Config::petConfig() { return pet_config; }
