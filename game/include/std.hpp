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

#define MASTER hl::singleton<std::shared_ptr<hl::game::master>>::get()
#define MAPS hl::singleton<::hl::game::map_factory>::get()

#endif //STREET_HOMELESS_SERVER_STD_HPP
