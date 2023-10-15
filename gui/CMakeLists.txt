cmake_minimum_required(VERSION 3.10)

project(gui LANGUAGES CXX C)



set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# set shader directory
set(SHADER_DIR ${SRC_DIR}/shaders)

#set font directory
set(FONT_DIR ${SRC_DIR}/fonts)

# set texture directory
set(TEXTURE_DIR ${SRC_DIR}/textures)

# set the destination directories

set(SHADER_DEST_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)
set(FONT_DEST_DIR ${CMAKE_CURRENT_BINARY_DIR}/fonts)
set(TEXTURE_DEST_DIR ${CMAKE_CURRENT_BINARY_DIR}/textures)

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

# # copy shaders to build directory
# file(COPY ${SHADER_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${FONT_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${TEXTURE_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# # copy imgui.ini to build directory
# file(COPY ${SRC_DIR}/default.layout DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${SRC_DIR}/default.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE LAYOUT_FILES RELATIVE ${SRC_DIR} ${SRC_DIR}/*.layout )
file(GLOB_RECURSE INI_FILES RELATIVE ${SRC_DIR} ${SRC_DIR}/*.ini )

file(GLOB_RECURSE SHADER_FILES RELATIVE ${SHADER_DIR} ${SHADER_DIR}/* )
file(GLOB_RECURSE FONT_FILES RELATIVE ${FONT_DIR} ${FONT_DIR}/* )
file(GLOB_RECURSE TEXTURE_FILES RELATIVE ${TEXTURE_DIR} ${TEXTURE_DIR}/* )



foreach( LAYOUT_FILE ${LAYOUT_FILES} )
    configure_file( ${SRC_DIR}/${LAYOUT_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${LAYOUT_FILE} COPYONLY )
endforeach()

foreach( INI_FILE ${INI_FILES} )
    configure_file( ${SRC_DIR}/${INI_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${INI_FILE} COPYONLY )
endforeach()

foreach( SHADER_FILE ${SHADER_FILES} )
    configure_file( ${SHADER_DIR}/${SHADER_FILE} ${SHADER_DEST_DIR}/${SHADER_FILE} COPYONLY )
endforeach()

foreach( FONT_FILE ${FONT_FILES} )
    configure_file( ${FONT_DIR}/${FONT_FILE} ${FONT_DEST_DIR}/${FONT_FILE} COPYONLY )
endforeach()

foreach( TEXTURE_FILE ${TEXTURE_FILES} )
    configure_file( ${TEXTURE_DIR}/${TEXTURE_FILE} ${TEXTURE_DEST_DIR}/${TEXTURE_FILE} COPYONLY )
endforeach()

configure_file( ${SRC_DIR}/icon.png ${CMAKE_CURRENT_BINARY_DIR}/icon.png COPYONLY )



# Add more lines for other resource types as needed


add_subdirectory(tests)



