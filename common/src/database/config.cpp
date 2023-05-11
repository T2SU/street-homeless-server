//
// Created by TSU on 2023-05-11.
//
#include "std_common.hpp"
#include "database/config.hpp"
#include "utils/yaml_config.hpp"

hl::database::config::config()
: _config(std::make_shared<sqlpp::mysql::connection_config>())
{
    load_config();
}

void hl::database::config::load_config()
{
    const auto config = hl::yaml::load("db_config.yaml");

    _config->database = config["database"].as<std::string>();
    _config->user = config["username"].as<std::string>();
    _config->password = config["password"].as<std::string>();
    _config->port = config["port"].as<uint32_t>();
    _pooling_min = config["pooling_min"].as<size_t>();
    _pooling_max = config["pooling_max"].as<size_t>();
    LOGD << "Loaded database configurations.";
}

std::shared_ptr<sqlpp::mysql::connection_config> hl::database::config::get() const
{
    return _config;
}

std::shared_ptr<sqlpp::mysql::connection_config> hl::database::config::operator()() const
{
    return _config;
}

size_t hl::database::config::get_pooling_min() const
{
    return _pooling_min;
}

size_t hl::database::config::get_pooling_max() const
{
    return _pooling_max;
}
