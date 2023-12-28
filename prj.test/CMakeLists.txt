set_property(TARGET porocheck PROPERTY CXX_STANDARD 17)

add_executable(main  "main.cpp")
add_executable(lib_test "lib_test.cpp" )

target_link_libraries(main opencv_core opencv_highgui unofficial::lemon::lemon)
target_link_libraries(lib_test  porocheck unofficial::lemon::lemon opencv_core opencv_highgui nlohmann_json::nlohmann_json)