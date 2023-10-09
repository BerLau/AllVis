cmake_minimum_required(VERSION 3.10)

project(gui LANGUAGES CXX C)

# set shader directory
set(SHADER_DIR src/shaders)

#set font directory
set(FONT_DIR src/fonts)

# set texture directory
set(TEXTURE_DIR src/textures)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# add all files to the executable
file(GLOB_RECURSE SOURCES 
    ${SRC_DIR}/*.cpp 
    ${SRC_DIR}/*.h
    ${SRC_DIR}/*.hpp
)

set(EXE_NAME ${PROJECT_NAME})
set(LIB_NAME ${PROJECT_NAME}lib)

# create executable
add_executable(${EXE_NAME} ${SOURCES})

# create library
add_library( ${LIB_NAME} STATIC ${SOURCES} )

#link libraries
target_link_libraries(${EXE_NAME} PRIVATE 
glad 
glfw 
imgui 
core 
FreeImage
freetype
math
)

target_link_libraries(${LIB_NAME} PRIVATE
glad
glfw
imgui
core
FreeImage
freetype
math
)

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# copy shaders to build directory
file(COPY ${SHADER_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${FONT_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${TEXTURE_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# copy imgui.ini to build directory
file(COPY ${SRC_DIR}/default.layout DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${SRC_DIR}/default.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(tests)



