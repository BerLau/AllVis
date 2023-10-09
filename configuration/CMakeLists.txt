cmake_minimum_required(VERSION 3.5)

# Set the project name
project (configuration)

# set a project version
set (PROJ_VERSION_MAJOR 0)
set (PROJ_VERSION_MINOR 2)
set (PROJ_ERSION_PATCH 1)
set (PROJ_VERSION "${PROJ_VERSION_MAJOR}.${PROJ_VERSION_MINOR}.${PROJ_ERSION_PATCH}")

# Call configure files on ver.h.in to set the version.
# Uses the standard ${VARIABLE} syntax in the file
configure_file(version.h.in ${PROJECT_BINARY_DIR}/version.h)

# configure the path.h.in file.
# This file can only use the @VARIABLE@ syntax in the file
configure_file(environment.h.in ${PROJECT_BINARY_DIR}/patenvironmenth.h @ONLY)

# Add an executable
add_library(${PROJECT_NAME} INTERFACE)

# include the directory with the new files
target_include_directories( ${PROJECT_NAME} INTERFACE ${CMAKE_BINARY_DIR} )