//
// Created by TSU on 2023-05-11.
//

#ifndef STREET_HOMELESS_SERVER_UTILS_HPP
#define STREET_HOMELESS_SERVER_UTILS_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "utils/hash.hpp"
#include "utils/mysql_transaction.hpp"
#include "utils/net_tools.hpp"
#include "utils/singleton.hpp"
#include "utils/synchronized.hpp"
#include "utils/yaml_config.hpp"
#include "utils/point.hpp"
#include "utils/random.hpp"

#endif //STREET_HOMELESS_SERVER_UTILS_HPP
