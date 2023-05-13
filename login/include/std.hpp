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
#include "messages.pb.h"
#include "account_data.pb.h"
#include "login_result.pb.h"
#include "player_data.pb.h"
#include "player_stat.pb.h"
#include "inventory.pb.h"
#include "player_appearance.pb.h"
#include "create_character_result.pb.h"
#include "character_list_result.pb.h"
#include "select_character_result.pb.h"
#include "delete_character_result.pb.h"
#include "homeless-db.h"

#endif //STREET_HOMELESS_SERVER_STD_HPP
