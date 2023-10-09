cmake_minimum_required(VERSION 3.10)

project(glad)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE 
    ${SRC_DIR}/glad.c
)

set(HEADERS
    ${INCLUDE_DIR}/glad/glad.h
    ${INCLUDE_DIR}/KHR/khrplatform.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

