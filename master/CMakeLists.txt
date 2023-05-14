add_executable(HomelessMaster
        src/main.cpp
        src/net/master_server.cpp
        src/net/master_session.cpp
        src/net/handler.cpp
        src/world/game_world.cpp
        src/handlers/setup_req.cpp
        src/handlers/enter_world_req.cpp
        src/users/change_map_req.cpp src/users/abstract_request.cpp src/world/map_state.cpp src/users/user_record.cpp src/users/db_user_job.cpp src/world/hangout.cpp)
target_include_directories(HomelessMaster PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(HomelessMaster HomelessCommon)
