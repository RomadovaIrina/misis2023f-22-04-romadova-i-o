cmake_minimum_required(VERSION 3.18)   

project ("temporary")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_VERBOSE_MAKEFILE True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True) 

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin.relwithdbg)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "..." FORCE)
endif()


find_package(OpenCV REQUIRED)
find_package(LEMON REQUIRED)

add_subdirectory ("prj.cw")
add_executable(temp  "temp.cpp" )
target_link_libraries(temp  porocheck unofficial::lemon::lemon opencv_core opencv_highgui)

add_executable(main  "main.cpp")


target_link_libraries(main opencv_core opencv_highgui)
target_link_libraries(main unofficial::lemon::lemon)

# Include sub-projects.
add_subdirectory ("prj.codeforces")
