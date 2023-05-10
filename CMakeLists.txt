cmake_minimum_required(VERSION 3.19)
project(street-homeless-server VERSION 1.0)

include(FetchContent)

if(WIN32)
set(ENV{PATH} "C:\\windows\\system32")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

#if(MSVC)
#    string (REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT}")
#    string (REPLACE "/Zi" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
#    add_compile_options(/W4 /WX /fsanitize=address /Z7)
#    add_compile_options(/WX /Z7)
#else()
#    add_compile_options(-Wall -Werror -Wextra -g)
#endif()


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(Protobuf REQUIRED)
add_subdirectory(street-homeless-protobuf)

set(PostgreSQL_ADDITIONAL_VERSIONS 15)
set(BUILD_POSTGRESQL_CONNECTOR ON)

# add_subdirectory(ddl2cpp)

add_subdirectory(sqlpp11)
add_subdirectory(plog)
add_subdirectory(libuv)
add_subdirectory(yaml-cpp)
add_subdirectory(common)
add_subdirectory(master)
add_subdirectory(login)
