//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_STD_HPP
#define STREET_HOMELESS_SERVER_STD_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include <homeless-common.hpp>
#include "homeless-db.h"

#define HANGOUT hl::singleton<hl::master::hangout>::get()
#define GAME_WORLD hl::singleton<hl::master::game_world>::get()

#endif //STREET_HOMELESS_SERVER_STD_HPP
