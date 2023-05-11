//
// Created by TSU on 2023-05-11.
//

#include "std_common.hpp"
#include "utils/yaml_config.hpp"

YAML::Node hl::yaml::load(const std::string &name)
{
    const char* config_root = std::getenv("HL_CONFIG_DIR");
    if (config_root == nullptr)
    {
        LOGW << "Not defined 'HL_CONFIG_DIR' environment variable, fallback to '../config'";
        config_root = "../config";
    }
    const auto path = std::filesystem::path(config_root) / name;
    return YAML::LoadFile(path.string());
}
