enable_testing()

set(EXE_NAME "test_${PROJECT_NAME}")

# add all .h and .cpp files in the current directory to the executable
file(GLOB_RECURSE TEST_SOURCES "*.cpp" "*.h")

# add the executable
add_executable(${EXE_NAME} ${TEST_SOURCES})

target_link_libraries(${EXE_NAME} ${PROJECT_NAME} gtest gtest_main)

gtest_discover_tests(${EXE_NAME})