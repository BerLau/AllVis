# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/oliver/Source/tutorial_cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oliver/Source/tutorial_cmake

# Include any dependencies generated for this target.
include math/tests/CMakeFiles/test_math.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include math/tests/CMakeFiles/test_math.dir/compiler_depend.make

# Include the progress variables for this target.
include math/tests/CMakeFiles/test_math.dir/progress.make

# Include the compile flags for this target's objects.
include math/tests/CMakeFiles/test_math.dir/flags.make

math/tests/CMakeFiles/test_math.dir/main.cpp.o: math/tests/CMakeFiles/test_math.dir/flags.make
math/tests/CMakeFiles/test_math.dir/main.cpp.o: math/tests/main.cpp
math/tests/CMakeFiles/test_math.dir/main.cpp.o: math/tests/CMakeFiles/test_math.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oliver/Source/tutorial_cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object math/tests/CMakeFiles/test_math.dir/main.cpp.o"
	cd /home/oliver/Source/tutorial_cmake/math/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT math/tests/CMakeFiles/test_math.dir/main.cpp.o -MF CMakeFiles/test_math.dir/main.cpp.o.d -o CMakeFiles/test_math.dir/main.cpp.o -c /home/oliver/Source/tutorial_cmake/math/tests/main.cpp

math/tests/CMakeFiles/test_math.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_math.dir/main.cpp.i"
	cd /home/oliver/Source/tutorial_cmake/math/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oliver/Source/tutorial_cmake/math/tests/main.cpp > CMakeFiles/test_math.dir/main.cpp.i

math/tests/CMakeFiles/test_math.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_math.dir/main.cpp.s"
	cd /home/oliver/Source/tutorial_cmake/math/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oliver/Source/tutorial_cmake/math/tests/main.cpp -o CMakeFiles/test_math.dir/main.cpp.s

# Object files for target test_math
test_math_OBJECTS = \
"CMakeFiles/test_math.dir/main.cpp.o"

# External object files for target test_math
test_math_EXTERNAL_OBJECTS =

math/tests/test_math: math/tests/CMakeFiles/test_math.dir/main.cpp.o
math/tests/test_math: math/tests/CMakeFiles/test_math.dir/build.make
math/tests/test_math: lib/libgtest.a
math/tests/test_math: lib/libgtest_main.a
math/tests/test_math: lib/libgtest.a
math/tests/test_math: math/tests/CMakeFiles/test_math.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/oliver/Source/tutorial_cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_math"
	cd /home/oliver/Source/tutorial_cmake/math/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_math.dir/link.txt --verbose=$(VERBOSE)
	cd /home/oliver/Source/tutorial_cmake/math/tests && /usr/local/bin/cmake -D TEST_TARGET=test_math -D TEST_EXECUTABLE=/home/oliver/Source/tutorial_cmake/math/tests/test_math -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/oliver/Source/tutorial_cmake/math/tests -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=test_math_TESTS -D CTEST_FILE=/home/oliver/Source/tutorial_cmake/math/tests/test_math[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /usr/local/share/cmake-3.27/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
math/tests/CMakeFiles/test_math.dir/build: math/tests/test_math
.PHONY : math/tests/CMakeFiles/test_math.dir/build

math/tests/CMakeFiles/test_math.dir/clean:
	cd /home/oliver/Source/tutorial_cmake/math/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_math.dir/cmake_clean.cmake
.PHONY : math/tests/CMakeFiles/test_math.dir/clean

math/tests/CMakeFiles/test_math.dir/depend:
	cd /home/oliver/Source/tutorial_cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oliver/Source/tutorial_cmake /home/oliver/Source/tutorial_cmake/math/tests /home/oliver/Source/tutorial_cmake /home/oliver/Source/tutorial_cmake/math/tests /home/oliver/Source/tutorial_cmake/math/tests/CMakeFiles/test_math.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : math/tests/CMakeFiles/test_math.dir/depend

