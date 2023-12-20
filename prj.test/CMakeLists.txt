set_property(TARGET porocheck PROPERTY CXX_STANDARD 17)

add_executable(main  "main.cpp")
add_executable(temp  "temp.cpp" )

target_link_libraries(main opencv_core opencv_highgui unofficial::lemon::lemon)
target_link_libraries(temp  porocheck unofficial::lemon::lemon opencv_core opencv_highgui)