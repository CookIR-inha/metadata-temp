# CMake 설정
cmake_minimum_required(VERSION 3.5)
project(SoftBound)

set(CMAKE_CXX_STANDARD 14)

# 1. LLVM 설정
find_package(LLVM 14 REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(LLVMConfig)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# 2. LLVM Pass 플러그인 생성
add_library(SoftBoundPass SHARED SoftBoundPass.cpp)
target_link_libraries(SoftBoundPass LLVM LLVMCore LLVMTransformUtils)
set_target_properties(SoftBoundPass PROPERTIES OUTPUT_NAME "SoftBoundPass")
target_compile_definitions(SoftBoundPass PRIVATE DEBUG) # 디버깅 활성화

# 3. test.ll 생성 명령
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/test.ll
    COMMAND clang -O0 -g -Xclang -disable-O0-optnone -emit-llvm -S ${CMAKE_SOURCE_DIR}/test.c -o ${CMAKE_BINARY_DIR}/test.ll
    COMMENT "Generating LLVM IR (test.ll) from test.c"
    VERBATIM
)

# 4. output.ll 생성 명령
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/output.ll
    COMMAND opt -load-pass-plugin ${CMAKE_BINARY_DIR}/libSoftBoundPass.so --passes=softbound -o ${CMAKE_BINARY_DIR}/output.ll ${CMAKE_BINARY_DIR}/test.ll
    DEPENDS ${CMAKE_BINARY_DIR}/test.ll
    COMMENT "Applying LLVM Pass Plugin with debugging to test.ll to generate output.ll"
    VERBATIM
)

# 5. IR 파일 생성 대상
add_custom_target(
    GenerateIR ALL
    DEPENDS ${CMAKE_BINARY_DIR}/output.ll
)

# 6. 공유 라이브러리 생성
## 기존 라이브러리 (softbound)
add_library(softbound SHARED softbound.c)
target_compile_options(softbound PRIVATE -fPIC)
set_target_properties(softbound PROPERTIES OUTPUT_NAME "softbound")
add_dependencies(softbound GenerateIR) # IR 파일 생성 후 라이브러리 생성

## 추가된 라이브러리 (softbound-wrapper)
add_library(softbound-wrapper SHARED softbound-wrapper.c)
target_compile_options(softbound-wrapper PRIVATE -fPIC)
set_target_properties(softbound-wrapper PROPERTIES OUTPUT_NAME "softbound-wrapper")

## 추가된 라이브러리
add_library(shadow_memory SHARED shadow_memory.c)
target_compile_options(shadow_memory PRIVATE -fPIC)
set_target_properties(shadow_memory PROPERTIES OUTPUT_NAME "shadow_memory")

# 7. output_binary 생성 명령
add_custom_target(
    output_binary ALL
    COMMAND clang ${CMAKE_BINARY_DIR}/output.ll -o ${CMAKE_BINARY_DIR}/output_binary -L. -lsoftbound-wrapper -lsoftbound -lshadow_memory -lm -Wl,-rpath,.
    DEPENDS ${CMAKE_BINARY_DIR}/output.ll softbound softbound-wrapper shadow_memory
    COMMENT "Linking output.ll to create output_binary"
)

# 8. 설치 경로 설정
install(TARGETS SoftBoundPass softbound softbound-wrapper
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(FILES ${CMAKE_BINARY_DIR}/output_binary
        DESTINATION bin)
