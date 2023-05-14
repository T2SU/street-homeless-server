add_library(HomelessCommon STATIC
        src/net/abstract_session.cpp
        src/net/network_exceptions.cpp
        src/utils/net_tools.cpp
        src/memory/memory_pool.cpp
        src/net/packet.cpp
        src/thread/socket_thread.cpp
        src/net/abstract_server.cpp
        src/net/packet_handler.cpp
        src/thread/socket_thread_pool.cpp
        src/net/socket_job.cpp
        src/std_common.cpp
        src/utils/hash.cpp
        src/utils/mysql_transaction.cpp
        src/database/config.cpp
        src/database/pool.cpp
        src/database/accessor.cpp
        src/database/job.cpp src/utils/yaml_config.cpp src/net/connector.cpp src/users/player_data.cpp src/utils/point.cpp)

include(FindOpenSSL)
find_package(OpenSSL REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(libuv REQUIRED)
find_package(sqlpp11 REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(fmt CONFIG REQUIRED)
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm64" AND APPLE)
    find_package(unofficial-libmariadb CONFIG REQUIRED)
    set(mariadblib unofficial::libmariadb)
    set(mariadbinc "")
else()
    find_package(MariaDBClient REQUIRED)
    set(mariadblib ${MariaDBClient_LIBRARIES})
    set(mariadbinc ${MariaDBClient_INCLUDE_DIRS})
endif()

include(FetchContent)
FetchContent_Declare(
        plog
        GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
)
FetchContent_MakeAvailable(plog)

target_include_directories(HomelessCommon PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include ${mariadbinc})
target_link_libraries(HomelessCommon PUBLIC
        OpenSSL::Crypto
        plog::plog
        yaml-cpp
        uv_a
        sqlpp11::sqlpp11
        PostgreSQL::PostgreSQL
        ${mariadblib}
        pb-messages
        sqlpp11::ddl
        fmt::fmt)
target_precompile_headers(HomelessCommon PUBLIC include/std_common.hpp)