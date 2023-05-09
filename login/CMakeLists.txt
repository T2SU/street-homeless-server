add_executable(HomelessLogin
        src/main.cpp
        src/net/login_session.cpp
        src/net/login_server.cpp)
get_property(PROTO_HEADER_DIR GLOBAL PROPERTY PROTO_HEADER_DIR_PROPERTY)
target_include_directories(HomelessLogin PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include ${PROTO_HEADER_DIR})
target_link_libraries(HomelessLogin plog yaml-cpp uv_a HomelessCommon ${PROTOBUF_LIBRARY} messages)