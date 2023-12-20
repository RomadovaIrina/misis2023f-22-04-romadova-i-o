add_library(porocheck porocheck.cpp include/porocheck/porocheck.hpp)
set_property(TARGET porocheck PROPERTY CXX_STANDARD 17)
target_include_directories(porocheck PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries( porocheck opencv_core opencv_highgui unofficial::lemon::lemon)

#target_link_libraries( porocheck PRIVATE opencv_core opencv_highgui )
install(TARGETS porocheck)

