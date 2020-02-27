cmake_minimum_required(VERSION 3.0.0)
project(maxconnect4 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

set(SFML_INCLUDE_DIR "../include")
set(SFML_LIBRARY_DIR "../lib")
set(SFML_DIR "../lib/cmake/SFML")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()

include_directories(include)
link_directories(lib)

include(CTest)
enable_testing()

add_executable(maxconnect4 max_connect_4.cpp)
target_link_libraries(maxconnect4 sfml-audio sfml-graphics sfml-system sfml-window sfml-network sfml-main)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
