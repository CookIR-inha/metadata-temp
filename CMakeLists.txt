#CMakeLists.txt
cmake_minimum_required(VERSION 3.5)
project(SoftBound)

set(CMAKE_CXX_STANDARD 14)

find_package(LLVM 14 REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(LLVMConfig)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(SoftBoundPass SHARED SoftBoundPass.cpp)

target_link_libraries(SoftBoundPass LLVM LLVMCore LLVMTransformUtils)
