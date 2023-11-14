# external libraries
cmake_minimum_required(VERSION 3.10)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glfw glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/freetype freetype)

# if glad is not found, abort
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glad)
    message(FATAL_ERROR "glad not found, donwload it from https://glad.dav1d.de/ and put it in the glad folder")
endif()

# # add the glad library
# add_library(glad STATIC
#     glad/src/glad.c
# )

# # add the include directory
# target_include_directories(glad PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
# )

# if glfw is not found, abort
if(GLFW_NOT_FOUND)
    message(FATAL_ERROR "glfw not found, using git clone https://github.com/glfw/glfw.git to download it")
    add_subdirectory(glfw)
endif()


# if imgui is not found, abort
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
    message(FATAL_ERROR "imgui not found, using git clone https://github.com/ocornut/imgui.git to download it")
endif()

# add stb_image
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/stb_image)

#add libtiff 
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libtiff)

#add OpenEXR
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/openexr)


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

