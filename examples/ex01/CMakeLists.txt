cmake_minimum_required(VERSION 3.5)

project(ex01 LANGUAGES CXX)

# add executable
add_executable(${PROJECT_NAME}
    ex01.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    core
    configuration
)

target_include_directories(ex01 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ex01
    ${CMAKE_SOURCE_DIR}/core/include
    ${CMAKE_SOURCE_DIR}/math/include
    ${PROJECT_BINARY_DIR}
)
