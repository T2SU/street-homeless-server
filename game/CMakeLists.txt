add_executable(HomelessGame
        src/main.cpp
        src/net/game_session.cpp
        src/net/master.cpp
        src/net/game_server.cpp
        src/std.cpp
        src/net/game_handler.cpp
        src/net/master_handler.cpp
        src/handlers/check_alive_res.cpp)

target_include_directories(HomelessGame PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(HomelessGame HomelessCommon)