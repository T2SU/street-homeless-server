cmake_minimum_required(VERSION 3.19)

project(db_proto)

#if(WIN32)
#    set(MYSQLDUMP_PATH "${CMAKE_CURRENT_LIST_DIR}/mysqldump.exe")
#else()
#    find_program(MYSQLDUMP_PATH mysqldump)
#endif()
#message( STATUS "MySQLDump Path=${MYSQLDUMP_PATH}")
#
#execute_process(COMMAND ${MYSQLDUMP_PATH} "-uroot" "-pfndndnxm"
#                OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/homeless-db.sql")

if(WIN32)
    set(PG_DUMP_PATH "${CMAKE_CURRENT_LIST_DIR}/pg_dump.exe")
else()
    find_program(PG_DUMP_PATH pg_dump)
endif()
message( STATUS "PG Dump Path=${PG_DUMP_PATH}")

set (ENV{PGPASSWORD} "fndndnxm")
execute_process(COMMAND ${PG_DUMP_PATH} "-U" "postgres" "-w" "homeless"
                OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/homeless-db.ddl")

include(FindPython3)
find_package(Python3 COMPONENTS Interpreter)
execute_process(COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_LIST_DIR}/ddl2cpp.py" "${CMAKE_CURRENT_BINARY_DIR}/homeless-db.ddl" "${CMAKE_CURRENT_BINARY_DIR}/homeless-db" "db")

add_library(db_proto INTERFACE)
message( STATUS "db_proto path=${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(db_proto INTERFACE "${CMAKE_CURRENT_BINARY_DIR}")