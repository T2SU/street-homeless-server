add_executable(HomelessLogin
        src/main.cpp
        src/net/login_session.cpp
        src/net/master.cpp
        src/net/login_server.cpp
        src/std.cpp
        src/net/login_handler.cpp
        src/handlers/check_alive_res.cpp
        src/handlers/login_req.cpp
        src/handlers/select_character_req.cpp src/handlers/create_character_req.cpp src/handlers/character_list_req.cpp src/handlers/delete_character_req.cpp src/net/master_handler.cpp src/mhandlers/change_map_res.cpp)

target_include_directories(HomelessLogin PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(HomelessLogin HomelessCommon)
target_precompile_headers(HomelessLogin PRIVATE include/std.hpp)