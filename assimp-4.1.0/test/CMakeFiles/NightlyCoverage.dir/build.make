# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/adrienhc/Downloads/assimp-4.1.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/adrienhc/Downloads/assimp-4.1.0

# Utility rule file for NightlyCoverage.

# Include the progress variables for this target.
include test/CMakeFiles/NightlyCoverage.dir/progress.make

test/CMakeFiles/NightlyCoverage:
	cd /home/adrienhc/Downloads/assimp-4.1.0/test && /usr/bin/ctest -D NightlyCoverage

NightlyCoverage: test/CMakeFiles/NightlyCoverage
NightlyCoverage: test/CMakeFiles/NightlyCoverage.dir/build.make

.PHONY : NightlyCoverage

# Rule to build all files generated by this target.
test/CMakeFiles/NightlyCoverage.dir/build: NightlyCoverage

.PHONY : test/CMakeFiles/NightlyCoverage.dir/build

test/CMakeFiles/NightlyCoverage.dir/clean:
	cd /home/adrienhc/Downloads/assimp-4.1.0/test && $(CMAKE_COMMAND) -P CMakeFiles/NightlyCoverage.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/NightlyCoverage.dir/clean

test/CMakeFiles/NightlyCoverage.dir/depend:
	cd /home/adrienhc/Downloads/assimp-4.1.0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/adrienhc/Downloads/assimp-4.1.0 /home/adrienhc/Downloads/assimp-4.1.0/test /home/adrienhc/Downloads/assimp-4.1.0 /home/adrienhc/Downloads/assimp-4.1.0/test /home/adrienhc/Downloads/assimp-4.1.0/test/CMakeFiles/NightlyCoverage.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/NightlyCoverage.dir/depend

