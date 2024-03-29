//
// Created by TSU on 2023-05-02.
//

#ifndef STD_COMMON_HPP
#define STD_COMMON_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#if _WIN32
// #define WIN32_LEAN_AND_MEAN
// #include <windows.h>
#endif

#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <queue>
#include <thread>
#include <chrono>
#include <memory>
#include <ranges>
#include <numeric>
#include <array>
#include <streambuf>
#include <exception>
#include <unordered_set>
#include <random>
#include <unordered_map>
#include <type_traits>
#include <algorithm>
#include <mutex>
#include <codecvt>
#include <regex>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Record.h>
#include <uv.h>
#include <openssl/sha.h>
#include <yaml-cpp/yaml.h>
#include <fmt/core.h>

#include "utils/synchronized.hpp"
#include "utils/singleton.hpp"
#include "utils/point.hpp"
#include "constants.hpp"

#include "resources/map_data.hpp"
#include "resources/region_info.hpp"

#undef min
#undef max
#include <sqlpp11/postgresql/postgresql.h>
#include <sqlpp11/mysql/mysql.h>
#include <sqlpp11/sqlpp11.h>

#define RAND ::hl::singleton<::hl::random>::get()

#endif //STD_COMMON_HPP
