add_executable(HomelessMaster
        src/main.cpp
        src/net/master_server.cpp
        src/net/master_session.cpp
        src/net/handler.cpp
        src/world/game_world.cpp
        src/handlers/setup_req.cpp
        src/handlers/migrate_region_req.cpp
        src/users/migration_region_request.cpp
        src/users/abstract_request.cpp
        src/world/region.cpp
        src/users/user_record.cpp
        src/users/db_user_job.cpp
        src/world/hangout.cpp
        src/handlers/on_migrate_req.cpp
        src/handlers/sign_out_req.cpp
        src/handlers/region_management_res.cpp)
target_include_directories(HomelessMaster PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(HomelessMaster HomelessCommon)
target_precompile_headers(HomelessMaster PRIVATE include/std.hpp)