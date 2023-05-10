add_executable(HomelessLogin
        src/main.cpp
        src/net/login_session.cpp
        src/net/login_server.cpp src/std.cpp)

target_include_directories(HomelessLogin PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(HomelessLogin HomelessCommon)