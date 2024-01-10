# external libraries
cmake_minimum_required(VERSION 3.10)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/freetype freetype)

# Configure GLAD

# if glad is not found, abort
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glad)
    message(FATAL_ERROR "glad not found, donwload it from https://glad.dav1d.de/ and put it in the glad folder")
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad glad)

# Configure GLFW

# if glfw folder is not found, download it
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glad)
    message(INFO "glfw not found, fetching it from https://github.com/glfw/glfw.git")
    execute_process(COMMAND git clone https://github.com/glfw/glfw.git ${CMAKE_CURRENT_SOURCE_DIR}/glfw)
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glfw glfw)
if(GLFW_NOT_FOUND)
    message(FATAL_ERROR "glfw configuration failed")
endif()

# Configure FreeType

# if freetype folder is not found, download it
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/freetype)
    message(INFO "freetype not found, fetching it from https://github.com/freetype/freetype.git")
    execute_process(COMMAND git clone https://github.com/freetype/freetype.git ${CMAKE_CURRENT_SOURCE_DIR}/freetype)
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/freetype freetype)

# Configure ImGUI

# if imgui folder is not found, download it
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
    message(INFO "imgui not found, fetch it from https://github.com/ocornut/imgui.git")
    execute_process(COMMAND git clone https://github.com/ocornut/imgui.git ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
    # switch to docking branch
    execute_process(COMMAND git checkout docking WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
endif()

# add stb_image
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/stb_image)

#add libtiff

# if libtiff folder is not found, download it
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libtiff)
    message(INFO "libtiff not found, fetching it from https://gitlab.com/libtiff/libtiff.git")
    execute_process(COMMAND git clone https://gitlab.com/libtiff/libtiff.git ${CMAKE_CURRENT_SOURCE_DIR}/libtiff)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libtiff)


set(IMGUI_SOURCES 
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

set(IMGUI_HEADERS
    imgui/imconfig.h
    imgui/imgui.h
    imgui/imgui_internal.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/backends/imgui_impl_glfw.h
    imgui/backends/imgui_impl_opengl3.h
)

# add the imgui library
add_library(imgui STATIC
    ${IMGUI_SOURCES}
    ${IMGUI_HEADERS}
)

# add the imgui include directory
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

