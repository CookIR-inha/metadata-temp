#ifndef SOFTBOUND_H
#define SOFTBOUND_H

#include <stddef.h>
#include <stdint.h>

// 색상 출력 관련 매크로
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

// 메타데이터 크기 정의
#define PRIMARY_TABLE_SIZE (1 << 25) // 48-25 비트
#define SECONDARY_TABLE_SIZE (1 << 20)

// 메타데이터 구조체 정의
typedef struct
{
    void *base;
    void *bound;
} Metadata;

// 전역 변수 선언
extern Metadata **primary_table;
extern size_t *__softboundcets_shadow_stack_ptr;

// 함수 선언

// 메모리 덤프 출력
void print_memory_dump(void *access, void *base, void *bound);

// 인덱스 계산 함수
size_t get_primary_index(void *ptr);
size_t get_secondary_index(void *ptr);

// 메타데이터 테이블 초기화
void _init_metadata_table(void);

// 메타데이터 저장 및 가져오기
void _softboundcets_set_metadata(void *ptr, void *base, void *bound);
void *__softboundcets_get_base_addr(void *access);
void *__softboundcets_get_bound_addr(void *access);

// 메타데이터 테이블 출력
void _softboundcets_print_metadata_table(void);

// 경계 검사 함수
void _softboundcets_bound_check(void *base, void *bound, void *access);
void _softboundcets_print_metadata(void *base, void *bound);

// Trie 관련 함수
void *__softboundcets_trie_allocate(void);

// Shadow Stack 관련 함수
Metadata *__softboundcets_shadow_stack_metadata_ptr(size_t arg_no);
Metadata *__softboundcets_shadowspace_metadata_ptr(void *address);

void __softboundcets_allocate_shadow_stack_space(size_t num_pointer_args);
void __softboundcets_deallocate_shadow_stack_space(void);

void __softboundcets_store_metadata_shadow_stack(void *base, void *bound, size_t arg_no);
void *__softboundcets_load_base_shadow_stack(size_t arg_no);
void *__softboundcets_load_bound_shadow_stack(size_t arg_no);
void __softboundcets_store_base_shadow_stack(void *base, size_t arg_no);
void __softboundcets_store_bound_shadow_stack(void *bound, size_t arg_no);

#endif // SOFTBOUND_H
