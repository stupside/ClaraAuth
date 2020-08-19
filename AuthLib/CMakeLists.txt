cmake_minimum_required(VERSION 2.8.12)
project(AuthLib)

#add_definitions("-std=c++11")

##### UNCOMMENT

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup(TARGETS)

##### UNCOMMENT


add_library(AuthLib STATIC AuthLib.cpp)
#target_link_libraries(Auth ${CONAN_LIBS})
