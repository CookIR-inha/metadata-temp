# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/test/metadata-temp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/test/metadata-temp/build

# Utility rule file for GenerateIR.

# Include any custom commands dependencies for this target.
include CMakeFiles/GenerateIR.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GenerateIR.dir/progress.make

CMakeFiles/GenerateIR: output.ll

output.ll: test.ll
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/test/metadata-temp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Applying LLVM Pass Plugin with debugging to test.ll to generate output.ll"
	/usr/lib/llvm-14/bin/opt -load-pass-plugin /home/test/metadata-temp/build/libSoftBoundPass.so --passes=softbound -o /home/test/metadata-temp/build/output.ll /home/test/metadata-temp/build/test.ll

test.ll:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/test/metadata-temp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating LLVM IR (test.ll) from test.c"
	clang -O0 -g -Xclang -disable-O0-optnone -emit-llvm -S /home/test/metadata-temp/test.c -o /home/test/metadata-temp/build/test.ll

GenerateIR: CMakeFiles/GenerateIR
GenerateIR: output.ll
GenerateIR: test.ll
GenerateIR: CMakeFiles/GenerateIR.dir/build.make
.PHONY : GenerateIR

# Rule to build all files generated by this target.
CMakeFiles/GenerateIR.dir/build: GenerateIR
.PHONY : CMakeFiles/GenerateIR.dir/build

CMakeFiles/GenerateIR.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GenerateIR.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GenerateIR.dir/clean

CMakeFiles/GenerateIR.dir/depend:
	cd /home/test/metadata-temp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/test/metadata-temp /home/test/metadata-temp /home/test/metadata-temp/build /home/test/metadata-temp/build /home/test/metadata-temp/build/CMakeFiles/GenerateIR.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/GenerateIR.dir/depend

