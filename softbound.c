// softbound.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <bits/mman-linux.h>

#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"
#define BYTES_PER_LINE 8
#include <assert.h>
#include "shadow_memory.h"

/*
주소의 희소성을 메모리 효율적 처리를 위해 2중 trie구조로 변경
2중 배열(primary table: 루트, secondary table: 동적 할당)
primary table: 대략적인 메모리 영역을 구분
secondary table: primary table내 세부 위치 구분
*/

// #define HASH_TABLE_SIZE (1 << 20) // 2^20
#define PRIMARY_TABLE_SIZE (1 << 25) // 48-25
#define SECONDARY_TABLE_SIZE (1 << 20)
static const size_t __SOFTBOUNDCETS_TRIE_SECONDARY_TABLE_ENTRIES =
    ((size_t)4 * (size_t)1024 * (size_t)1024);
static const size_t __SOFTBOUNDCETS_SHADOW_STACK_ENTRIES =
    ((size_t)128 * (size_t)32);

typedef struct
{
  void *base;
  void *bound;
} Metadata;

// Metadata metadata_table[HASH_TABLE_SIZE];
// Metadata *metadata_table = NULL;
Metadata **primary_table = NULL;
size_t *__softboundcets_shadow_stack_ptr = NULL;

void print_memory_dump(void *access, void *base, void *bound)
{
  unsigned char *start = (unsigned char *)base;
  unsigned char *end = (unsigned char *)bound;
  unsigned char *access_addr = (unsigned char *)access;

  printf("==================================================================\n");

  for (unsigned char *current_addr = (unsigned char *)((uintptr_t)start & ~0xF);
       current_addr < end + 0x100;
       current_addr += 0x10)
  {
    printf("%p: ", current_addr);

    // 16바이트 데이터 출력
    for (int i = 0; i < 16; i++)
    {
      unsigned char *byte_addr = current_addr + i;

      if (byte_addr >= start && byte_addr < end) // 범위 내
      {
        printf(GREEN "%02x " RESET, *byte_addr);
      }
      else // 범위 외
      {
        printf(BLUE "%02x " RESET, *byte_addr);
      }
    }

    // `access` 주소에 맞게 `^` 위치 계산
    if (current_addr <= access_addr && access_addr < current_addr + 0x10)
    {
      int offset = access_addr - current_addr;                   // `access`의 위치 계산
      printf("\n%*s" RED "^" RESET "\n", 16 + (offset * 3), ""); // 동적 위치 계산
    }
    else
    {
      printf("\n");
    }
  }

  printf("==================================================================\n");
}

size_t get_primary_index(void *ptr)
{
  return (uintptr_t)ptr >> 25; // 상위 23비트 사용
}

size_t get_secondary_index(void *ptr)
{
  return ((size_t)ptr >> 3) & 0x3fffff;
}

void _softboundcets_init_metadata_table()
{
  printf("initializing table\n");
  primary_table = (Metadata **)mmap(NULL, sizeof(Metadata) * PRIMARY_TABLE_SIZE,
                                    PROT_READ | PROT_WRITE,
                                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (primary_table == MAP_FAILED)
  {
    printf("error table\n");
  }
  printf("Primary table initialization done\n");
  size_t shadow_stack_size =
      __SOFTBOUNDCETS_SHADOW_STACK_ENTRIES * sizeof(size_t);
  __softboundcets_shadow_stack_ptr =
      (size_t *)mmap(0, shadow_stack_size,
                     PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);

  assert(__softboundcets_shadow_stack_ptr != (void *)-1);
  // printf("shadow_stack_address : %p\n", __softboundcets_shadow_stack_ptr);
  *((size_t *)__softboundcets_shadow_stack_ptr) = 0; /* prev stack size */
  size_t *current_size_shadow_stack_ptr = __softboundcets_shadow_stack_ptr + 1;
  *(current_size_shadow_stack_ptr) = 0;
}

void *__softboundcets_trie_allocate()
{
  Metadata *secondary_entry;
  size_t length = (__SOFTBOUNDCETS_TRIE_SECONDARY_TABLE_ENTRIES) * sizeof(Metadata);
  secondary_entry = (Metadata *)mmap(0, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  return secondary_entry;
}

void _softboundcets_set_metadata(void *ptr, void *base, void *bound)
{
  size_t primary_index = get_primary_index(ptr);
  size_t secondary_index = get_secondary_index(ptr);
  Metadata *secondary_table = primary_table[primary_index];
  if (secondary_table == NULL)
  {
    secondary_table = __softboundcets_trie_allocate();
    primary_table[primary_index] = secondary_table;
  }
  Metadata *entry = &secondary_table[secondary_index];
  entry->base = base;
  entry->bound = bound;
  // printf("Stored Malloc Info - base: %p, bound: %p\n",
  //        primary_table[primary_index][secondary_index].base,
  //        primary_table[primary_index][secondary_index].bound);
  return;
}

void *__softboundcets_get_base_addr(void *access)
{
  size_t primary_index = get_primary_index(access);
  size_t secondary_index = get_secondary_index(access);
  void *base = primary_table[primary_index][secondary_index].base;
  return base;
}
void *__softboundcets_get_bound_addr(void *access)
{
  size_t primary_index = get_primary_index(access);
  size_t secondary_index = get_secondary_index(access);
  void *bound = primary_table[primary_index][secondary_index].bound;
  return bound;
}

void _softboundcets_print_metadata_table()
{
  printf("Printing non-empty entries in metadata table:\n");
  for (size_t i = 0; i < PRIMARY_TABLE_SIZE; i++)
  {
    if (primary_table[i])
    {
      for (size_t j = 0; j < SECONDARY_TABLE_SIZE; j++)
      {
        if (primary_table[i][j].base != NULL || primary_table[i][j].bound != NULL)
        {
          printf("Primary %zu, Secondary %zu -> Base: %p, Bound: %p\n",
                 i, j, primary_table[i][j].base, primary_table[i][j].bound);
        }
      }
    }
  }
}

void _softboundcets_bound_check(void *base, void *bound, void *access)
{
  if (!base)
    return;
  bool OOB = false;
  bool UAF = false;
  if (stack_addr < base)
  {
    // stack OOB detection
    if (bound <= access)
    {
      OOB = true;
      printf("***out-of-bound detected***\n");
      printf("accessing : %p, bound is : %p\n", access, bound);
      print_memory_dump(access, base, bound);
    }
  }
  else
  {
    // heap OOB or UAF detection
    size_t size = (uintptr_t)access - (uintptr_t)base;
    validate_memory_access(access, size);
  }

  // int8_t *shadow_addr = get_shadow_address(access);
  // size_t shadow_block_offset = get_shadow_block_offset(access);

  // /*
  // 메모리 접근은 할당과 달리 8바이트 정렬이 보장되지 않음
  // 다양한 메모리 접근 case가 있음(1블록 접근, 여러 블록 접근, 8바이트 정렬된 접근, 정렬되지 않은 접근..)
  // 따라서 일반화를 위해 첫 블록, 중간 블록, 마지막 블록으로 나누어 처리해야 함
  // 예시)
  // 섀도우 메모리 오프셋: 0x0
  // 메모리 접근 주소와 사이즈: 0x4, 32byte
  // 전체 접근 주소: 0x04-0x23 (32byte)
  // 첫 블록의 접근 주소: 0x4-0x7 (4byte)
  // 중간 블록의 접근 주소: 0x8-0xf, 0x10-0x17, 0x18-0x1f (24byte)
  // 마지막 블록의 접근 주소: 0x20-0x23 (4byte)

  // 이 경우 첫 블록에서는 마지막 4바이트만 접근하지만
  // 할당할 때 8바이트 정렬된 주소로부터 연속으로 할당되었으므로 해당 블록의 8바이트 전체가 유효해야만 접근 가능함
  // 중간 블록의 경우 항상 8바이트 전체가 유효해야 접근 가능함
  // 마지막 블록의 경우 첫 4바이트만 유효하면 접근 가능함
  // */

  // // 첫 블록에서 유효해야 하는 바이트 크기
  // printf("access is : %p\n", access);
  // printf("base is : %p\n", base);

  // addr = validate_memory_access(access, size);
  // printf("address is : %p\n",addr);

  // int32_t first_bytes = shadow_block_offset + size;
  // if (first_bytes > 8)
  //   first_bytes = 8;

  // int32_t remaining_size = size;
  // // 첫 블록에서 접근한 만큼 빼줌.
  // // 접근 가능한지 확인
  // if (first_bytes > shadow_addr[0])
  // {
  //   UAF = true;
  //   // report_error(access, size, shadow_addr);
  //   // return;
  // }
  // else if (first_bytes == 8)
  // {
  //   remaining_size = remaining_size - (8 - shadow_block_offset);
  // }
  // else
  // { // 접근 영역 전체가 1블록을 초과하지 않는다면 더 이상 진행할 필요 없으므로 size = 0
  //   remaining_size = 0;
  // }

  // // 중간 블록에 대해 접근 가능한지 확인
  // size_t i = 1;
  // int8_t *addr;
  // for (; remaining_size >= 8; i++, remaining_size -= 8)
  // {
  //   if (shadow_addr[i] != 8)
  //   {
  //     UAF = true;
  //     addr = &shadow_addr[i];
  //     break;
  //     // report_error(access, size, &shadow_addr[i]);
  //   }
  // }

  // // 마지막 블록에 대해 접근 가능한지 확인
  // if (remaining_size > shadow_addr[i])
  // {
  //   UAF = true;
  //   addr = &shadow_addr[i];
  //   // report_error(access, size, &shadow_addr[i]);
  // }
}
void _softboundcets_print_metadata(void *base, void *bound)
{
  printf("base address: %p\n", base);
  printf("bound address: %p\n", bound);
}

Metadata *__softboundcets_shadow_stack_metadata_ptr(size_t arg_no)
{
  assert(arg_no >= 0);
  size_t count = 2 + arg_no * 2;
  size_t *metadata_ptr = (__softboundcets_shadow_stack_ptr + count);
  return (Metadata *)metadata_ptr;
}

Metadata *__softboundcets_shadowspace_metadata_ptr(void *address)
{

  size_t ptr = (size_t)address;
  Metadata *trie_secondary_table;
  size_t primary_index = (ptr >> 25);
  trie_secondary_table = primary_table[primary_index];

  size_t secondary_index = ((ptr >> 3) & 0x3fffff);
  Metadata *entry_ptr =
      &trie_secondary_table[secondary_index];

  return entry_ptr;
}
void __softboundcets_allocate_shadow_stack_space(size_t num_pointer_args)
{
  size_t *prev_stack_size_ptr = __softboundcets_shadow_stack_ptr + 1;
  size_t prev_stack_size = *((size_t *)prev_stack_size_ptr);

  __softboundcets_shadow_stack_ptr =
      __softboundcets_shadow_stack_ptr + prev_stack_size + 2;

  *((size_t *)__softboundcets_shadow_stack_ptr) = prev_stack_size;
  size_t *current_stack_size_ptr = __softboundcets_shadow_stack_ptr + 1;

  ssize_t size = num_pointer_args * 2;
  *((size_t *)current_stack_size_ptr) = size;
}
void __softboundcets_deallocate_shadow_stack_space(void)
{
  size_t *reserved_space_ptr = __softboundcets_shadow_stack_ptr;

  size_t read_value = *((size_t *)reserved_space_ptr);
  __softboundcets_shadow_stack_ptr =
      __softboundcets_shadow_stack_ptr - read_value - 2;
}

void __softboundcets_store_metadata_shadow_stack(void *base, void *bound, size_t arg_no)
{
  Metadata *metadata = __softboundcets_shadow_stack_metadata_ptr(arg_no);
  metadata->base = base;
  metadata->bound = bound;
}

void *__softboundcets_load_base_shadow_stack(size_t arg_no)
{
  assert(arg_no >= 0);
  size_t count =
      2 + arg_no * 2 + 0; // number of field: 2, base index : 0
  size_t *base_ptr = (__softboundcets_shadow_stack_ptr + count);
  void *base = *((void **)base_ptr);
  return base;
}

void *__softboundcets_load_bound_shadow_stack(size_t arg_no)
{

  assert(arg_no >= 0);
  size_t count =
      2 + arg_no * 2 + 1; // number of field: 2, bound index : 1
  size_t *bound_ptr = (__softboundcets_shadow_stack_ptr + count);

  void *bound = *((void **)bound_ptr);
  return bound;
}
void __softboundcets_store_base_shadow_stack(void *base,
                                             size_t arg_no)
{
  printf("storing malloc's base\n");
  assert(arg_no >= 0);
  size_t count =
      2 + arg_no * 2 + 0;
  void **base_ptr = (void **)(__softboundcets_shadow_stack_ptr + count);

  *(base_ptr) = base;
}
void __softboundcets_store_bound_shadow_stack(void *bound,
                                              size_t arg_no)
{
  printf("storing malloc's bound\n");
  assert(arg_no >= 0);
  size_t count =
      2 + arg_no * 2 + 1;
  void **bound_ptr = (void **)(__softboundcets_shadow_stack_ptr + count);

  *(bound_ptr) = bound;
}
