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

# Include any dependencies generated for this target.
include CMakeFiles/SoftBoundPass.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/SoftBoundPass.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/SoftBoundPass.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SoftBoundPass.dir/flags.make

CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.o: CMakeFiles/SoftBoundPass.dir/flags.make
CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.o: ../SoftBoundPass.cpp
CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.o: CMakeFiles/SoftBoundPass.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/test/metadata-temp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.o -MF CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.o.d -o CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.o -c /home/test/metadata-temp/SoftBoundPass.cpp

CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/test/metadata-temp/SoftBoundPass.cpp > CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.i

CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/test/metadata-temp/SoftBoundPass.cpp -o CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.s

# Object files for target SoftBoundPass
SoftBoundPass_OBJECTS = \
"CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.o"

# External object files for target SoftBoundPass
SoftBoundPass_EXTERNAL_OBJECTS =

libSoftBoundPass.so: CMakeFiles/SoftBoundPass.dir/SoftBoundPass.cpp.o
libSoftBoundPass.so: CMakeFiles/SoftBoundPass.dir/build.make
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVM-14.so.1
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMCore.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMTransformUtils.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMAnalysis.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMProfileData.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMDebugInfoDWARF.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMObject.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMBitReader.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMCore.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMRemarks.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMBitstreamReader.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMMCParser.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMTextAPI.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMMC.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMBinaryFormat.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMDebugInfoCodeView.a
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMSupport.a
libSoftBoundPass.so: /usr/lib/x86_64-linux-gnu/libz.so
libSoftBoundPass.so: /usr/lib/x86_64-linux-gnu/libtinfo.so
libSoftBoundPass.so: /usr/lib/llvm-14/lib/libLLVMDemangle.a
libSoftBoundPass.so: CMakeFiles/SoftBoundPass.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/test/metadata-temp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libSoftBoundPass.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SoftBoundPass.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SoftBoundPass.dir/build: libSoftBoundPass.so
.PHONY : CMakeFiles/SoftBoundPass.dir/build

CMakeFiles/SoftBoundPass.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SoftBoundPass.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SoftBoundPass.dir/clean

CMakeFiles/SoftBoundPass.dir/depend:
	cd /home/test/metadata-temp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/test/metadata-temp /home/test/metadata-temp /home/test/metadata-temp/build /home/test/metadata-temp/build /home/test/metadata-temp/build/CMakeFiles/SoftBoundPass.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SoftBoundPass.dir/depend

