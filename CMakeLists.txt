cmake_minimum_required(VERSION 3.18)   

project ("PoroMarker")

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
find_package(Doxygen REQUIRED)
find_package(nlohmann_json REQUIRED)


option(BUILD_LIB "Build only library " OFF)
if(BUILD_LIB)
    add_subdirectory("prj.cw")
else()
    add_subdirectory("prj.cw")
    add_subdirectory ("prj.codeforces")
endif()

#add_subdirectory ("prj.cw")
add_subdirectory ("prj.test")
#add_subdirectory ("prj.codeforces")

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/prj.cw/Doxyfile)
    set(DOXYGEN_CONFIGURED_INPUT ${DOXYGEN_IN})
    
    add_custom_target(doc ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIGURED_INPUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Documentation with doxygen"
    VERBATIM )
endif()

if(DOXYGEN_FOUND)
       install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/documentation
               DESTINATION docs)
   endif()

install(TARGETS lib_test
DESTINATION bin)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/test_data/
  DESTINATION test_data
)

install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/
      DESTINATION bin
      CONFIGURATIONS Debug
      FILES_MATCHING PATTERN "*.dll*"
      )
  install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/
      CONFIGURATIONS Release
      DESTINATION bin
      FILES_MATCHING PATTERN "*.dll*"
      )
  install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}/
      DESTINATION bin
      CONFIGURATIONS RelWithDebInfo
      FILES_MATCHING PATTERN "*.dll*"
      )