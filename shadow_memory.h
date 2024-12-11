//shadow_memory.h

#ifndef SHADOW_MEMORY_H
#define SHADOW_MEMORY_H
#define SHADOW_SCALE 3
#define SHADOW_OFFSET 0x100000000000ULL // 2^44
#define SHADOW_SIZE 1ULL << (47 - SHADOW_SCALE) // 2^44 bytes (16TiB)
#include <stdint.h>
#include <stddef.h>

//섀도우 메모리 포인터
static int8_t* shadow_memory = (int8_t*)SHADOW_OFFSET;

void allocate_shadow_memory();

void free_shadow_memory();

//프로그램 메모리 주소 받아서 섀도우 메모리 주소 계산
static inline int8_t* get_shadow_address(void* addr) {
    return shadow_memory + (((uintptr_t)addr) >> SHADOW_SCALE);
}

//프로그램 메모리 주소가 섀도우 메모리 블록의 8바이트 중 몇번째에 해당하는지 리턴(0~7)
//즉, 메모리 주소를 8로 나눈 나머지
static inline size_t get_shadow_block_offset(void* addr) {
    return ((uintptr_t)addr) & ((1 << SHADOW_SCALE) - 1);
}

//프로그램 메모리 크기를 섀도우 메모리 크기로 변환
//1-8이면 1, 9-16이면 2 ...
static inline size_t get_shadow_size(size_t size) {
    return ((size - 1) >> SHADOW_SCALE) + 1;
}

void set_shadow_enc_alloc(void* addr, size_t size);

void set_shadow_enc_free(void* addr, size_t size);

void after_malloc(void* addr, size_t size);

void after_calloc(void* addr, size_t num, size_t size);

void after_realloc(void* old_addr, void* new_addr, size_t old_size, size_t new_size);

void after_posix_memalign(int result, void* addr, size_t size);

void after_free(void* addr, size_t size);


//void* wrapper_malloc(size_t size);
//void* wrapper_calloc(size_t num, size_t size);

//void wrapper_free(void* addr, size_t size);

void report_error(void* start_address, size_t size, int8_t* faulty_shadow_addr);

void validate_memory_access(void* addr, int32_t size);

#endif