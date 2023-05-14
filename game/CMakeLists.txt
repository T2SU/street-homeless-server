add_executable(HomelessGame
        src/main.cpp
        src/net/game_session.cpp
        src/net/master.cpp
        src/net/game_server.cpp
        src/std.cpp
        src/net/game_handler.cpp
        src/net/master_handler.cpp
        src/handlers/check_alive_res.cpp src/mhandlers/map_management_req.cpp src/maps/map.cpp src/maps/map_factory.cpp src/users/player.cpp src/users/player_stat.cpp src/item/item.cpp src/handlers/enter_game_world_req.cpp src/mhandlers/enter_game_res.cpp src/mhandlers/change_map_res.cpp)

target_include_directories(HomelessGame PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(HomelessGame HomelessCommon)
target_precompile_headers(HomelessGame PRIVATE include/std.hpp)