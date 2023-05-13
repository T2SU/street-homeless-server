add_executable(HomelessMaster src/main.cpp src/net/master_server.cpp src/net/master_session.cpp src/net/handler.cpp src/world/game_world.cpp src/handlers/setup_req.cpp)
target_include_directories(HomelessMaster PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(HomelessMaster HomelessCommon)
