cmake_minimum_required(VERSION 3.10)


list(APPEND CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/")
# print CMAKE_PREFIX_PATH
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")



# set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# set the project name and version
project(Tutorial VERSION 0.1 LANGUAGES CXX C) 


# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_MOUDULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Set made for debug
set(CMAKE_BUILD_TYPE Debug)

# set up google test

find_package(GTest)
if(NOT GTEST_FOUND)
    message(STATUS "GTest not found, fetching from github")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY  https://github.com/google/googletest.git
        GIT_TAG         release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
else()
    message(STATUS "GTest found")
    add_library(GTest::GTest ALIAS gtest)
    add_library(GTest::Main ALIAS gtest_main)
endif()

enable_testing()

# add external
add_subdirectory(external)
add_subdirectory(configuration)
add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(examples)





# install(TARGETS ex01 DESTINATION bin)
# install(TARGETS core DESTINATION lib)
# install(TARGETS math DESTINATION lib)

# install(DIRECTORY ${PROJECT_SOURCE_DIR}/core/include/ DESTINATION include)
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/math/ DESTINATION include)

# install(FILES ${PROJECT_SOURCE_DIR}/examples/ex01/ex01.cpp DESTINATION src)
# install(FILES project.conf DESTINATION etc)