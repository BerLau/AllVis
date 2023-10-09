enable_testing()

set(EXE_NAME "test_${LIB_NAME}")

# add all .h and .cpp files in the current directory to the executable
file(GLOB_RECURSE TEST_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# add the executable
add_executable(${EXE_NAME} ${TEST_FILES})

target_link_libraries(${EXE_NAME} 
    gtest 
    gtest_main
    ${LIB_NAME}
    imgui
    glfw
    glad
    freetype
    FreeImage
    core
)

gtest_discover_tests(${EXE_NAME})


