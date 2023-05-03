add_executable(HomelessMaster src/main.cpp)
target_include_directories(HomelessMaster PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(HomelessMaster plog yaml-cpp uv_a HomelessCommon)