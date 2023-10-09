cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(physics LANGUAGES CXX)

# add library
add_library(${PROJECT_NAME} INTERFACE)

# set target properties
target_link_libraries(${PROJECT_NAME} INTERFACE cxx_compiler_flags)

# add include directories
target_include_directories(${PROJECT_NAME} INTERFACE
 ${CMAKE_CURRENT_SOURCE_DIR})

 add_subdirectory(tests)