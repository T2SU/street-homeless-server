add_executable(HomelessLogin
        src/main.cpp
        src/net/login_session.cpp
        src/net/login_server.cpp src/std.cpp)

include(FindOpenSSL)
find_package(OpenSSL REQUIRED)

target_include_directories(HomelessLogin PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(HomelessLogin
        plog
        yaml-cpp
        uv_a
        sqlpp11::sqlpp11
        sqlpp11::postgresql
        OpenSSL::Crypto
        HomelessCommon
        ${PROTOBUF_LIBRARY}
        messages
        db_proto)