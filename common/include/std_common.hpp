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

#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <queue>
#include <thread>
#include <format>
#include <memory>
#include <array>
#include <exception>
#include <unordered_set>
#include <unordered_map>
#include <type_traits>
#include <algorithm>
#include <mutex>
#include <cstdio>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <uv.h>
#include "utils/synchronized.hpp"
#include "utils/singleton.hpp"
#include "constants.hpp"

#endif //STD_COMMON_HPP
