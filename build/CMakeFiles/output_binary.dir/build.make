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

# Utility rule file for output_binary.

# Include any custom commands dependencies for this target.
include CMakeFiles/output_binary.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/output_binary.dir/progress.make

CMakeFiles/output_binary: output.ll
CMakeFiles/output_binary: libsoftbound.so
CMakeFiles/output_binary: libsoftbound-wrapper.so
CMakeFiles/output_binary: libshadow_memory.so
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/test/metadata-temp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Linking output.ll to create output_binary"
	clang /home/test/metadata-temp/build/output.ll -o /home/test/metadata-temp/build/output_binary -L. -lsoftbound-wrapper -lsoftbound -lshadow_memory -lm -Wl,-rpath,.

output.ll: test.ll
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/test/metadata-temp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Applying LLVM Pass Plugin with debugging to test.ll to generate output.ll"
	/usr/lib/llvm-14/bin/opt -load-pass-plugin /home/test/metadata-temp/build/libSoftBoundPass.so --passes=softbound -o /home/test/metadata-temp/build/output.ll /home/test/metadata-temp/build/test.ll

test.ll:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/test/metadata-temp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating LLVM IR (test.ll) from test.c"
	clang -O0 -g -Xclang -disable-O0-optnone -emit-llvm -S /home/test/metadata-temp/test.c -o /home/test/metadata-temp/build/test.ll

output_binary: CMakeFiles/output_binary
output_binary: output.ll
output_binary: test.ll
output_binary: CMakeFiles/output_binary.dir/build.make
.PHONY : output_binary

# Rule to build all files generated by this target.
CMakeFiles/output_binary.dir/build: output_binary
.PHONY : CMakeFiles/output_binary.dir/build

CMakeFiles/output_binary.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/output_binary.dir/cmake_clean.cmake
.PHONY : CMakeFiles/output_binary.dir/clean

CMakeFiles/output_binary.dir/depend:
	cd /home/test/metadata-temp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/test/metadata-temp /home/test/metadata-temp /home/test/metadata-temp/build /home/test/metadata-temp/build /home/test/metadata-temp/build/CMakeFiles/output_binary.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/output_binary.dir/depend

