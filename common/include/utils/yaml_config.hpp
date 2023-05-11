//
// Created by TSU on 2023-05-11.
//

#ifndef STREET_HOMELESS_SERVER_YAML_CONFIG_HPP
#define STREET_HOMELESS_SERVER_YAML_CONFIG_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::yaml
{
    YAML::Node load(const std::string& name);
}

#endif //STREET_HOMELESS_SERVER_YAML_CONFIG_HPP
