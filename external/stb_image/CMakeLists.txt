cmake_minimum_required(VERSION 3.12)

#disable warings for stb_image
add_compile_options(-w)

add_library(stb_image  INTERFACE)

target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})