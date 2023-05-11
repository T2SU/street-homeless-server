add_executable(HomelessLogin
        src/main.cpp
        src/net/login_session.cpp
        src/net/login_server.cpp
        src/std.cpp
        src/net/handler.cpp
        src/handlers/check_alive_res.cpp
        src/handlers/login_req.cpp
        src/handlers/select_character_req.cpp)

target_include_directories(HomelessLogin PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(HomelessLogin HomelessCommon)