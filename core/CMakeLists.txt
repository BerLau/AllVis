cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(core LANGUAGES CXX)

# add all source files
file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
)

# add library

add_library(${PROJECT_NAME} STATIC ${SOURCES})

# set target properties
# target_link_libraries(${PROJECT_NAME} INTERFACE cxx_compiler_flags
# )

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# add_subdirectory(tests)