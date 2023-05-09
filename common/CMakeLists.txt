add_library(HomelessCommon STATIC
        src/net/abstract_session.cpp
        src/net/network_exceptions.cpp
        src/utils/net_tools.cpp
        src/memory/memory_pool.cpp
        src/net/packet.cpp
        src/thread/socket_thread.cpp
        src/net/abstract_server.cpp
        src/thread/socket_thread_pool.cpp
        src/net/socket_job.cpp)
get_property(PROTO_HEADER_DIR GLOBAL PROPERTY PROTO_HEADER_DIR_PROPERTY)
target_include_directories(HomelessCommon PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include ${PROTO_HEADER_DIR})
target_link_libraries(HomelessCommon plog yaml-cpp uv_a messages)