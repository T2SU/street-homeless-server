//
// Created by TSU on 2023-05-17.
//

#include "std_common.hpp"
#include "config/config.hpp"
#include "utils/yaml_config.hpp"

auto node = hl::yaml::load("data.yaml");
const std::string hl::config::data_root = node["resources"]["root"].as<std::string>();