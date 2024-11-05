// softbound.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <bits/mman-linux.h>

/*
주소의 희소성을 메모리 효율적 처리를 위해 2중 trie구조로 변경
2중 배열(primary table: 루트, secondary table: 동적 할당)
primary table: 대략적인 메모리 영역을 구분
secondary table: primary table내 세부 위치 구분
*/

// #define HASH_TABLE_SIZE (1 << 20) // 2^20
#define PRIMARY_TABLE_SIZE (1 << 25) // 48-25
#define SECONDARY_TABLE_SIZE (1 << 20)

typedef struct
{
  void *base;
  void *bound;
} Metadata;

// Metadata metadata_table[HASH_TABLE_SIZE];
// Metadata *metadata_table = NULL;
Metadata **primary_table = NULL;

size_t get_primary_index(void *ptr)
{
  return (uintptr_t)ptr >> 25; // 상위 23비트 사용
}

size_t get_secondary_index(void *ptr)
{
  return ((uintptr_t)ptr >> 4) & (SECONDARY_TABLE_SIZE - 1); // 하위 20비트 사용
}

void set_metadata(void *ptr, size_t size)
{
  size_t primary_index = get_primary_index(ptr);
  size_t secondary_index = get_secondary_index(ptr);

  if (!primary_table[primary_index])
  {
    primary_table[primary_index] = mmap(NULL, sizeof(Metadata) * SECONDARY_TABLE_SIZE,
                                        PROT_READ | PROT_WRITE,
                                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (primary_table[primary_index] == MAP_FAILED)
    {
      perror("mmap failed");
      exit(1);
    }

    // 새로 할당된 2차 테이블 초기화
    for (size_t i = 0; i < SECONDARY_TABLE_SIZE; i++)
    {
      primary_table[primary_index][i].base = NULL;
      primary_table[primary_index][i].bound = NULL;
    }
  }

  primary_table[primary_index][secondary_index].base = ptr;
  primary_table[primary_index][secondary_index].bound = (char *)ptr + size;
  printf("Stored Malloc Info - base: %p, bound: %p\n",
         primary_table[primary_index][secondary_index].base,
         primary_table[primary_index][secondary_index].bound);
}

bool get_metadata(void *ptr)
{
  size_t primary_index = get_primary_index(ptr);
  size_t secondary_index = get_secondary_index(ptr);

  if (!primary_table[primary_index])
  {
    return false; // 2차 테이블이 할당되지 않은 경우
  }

  Metadata metadata = primary_table[primary_index][secondary_index];
  return (metadata.base != NULL && metadata.bound != NULL);
}

void print_metadata_table()
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

void bound_check(void *ptr, void *access, const char *inst)
{
  size_t primary_index = get_primary_index(ptr);
  size_t secondary_index = get_secondary_index(ptr);

  if (!primary_table[primary_index])
  {
    printf("Error: Metadata not found for pointer %p\n", ptr);
    return;
  }

  Metadata metadata = primary_table[primary_index][secondary_index];
  if (metadata.base == NULL || metadata.bound == NULL)
  {
    printf("Error: Metadata not found for pointer %p\n", ptr);
    return;
  }

  printf("Memory access at %p for pointer %p[%s]\n", access, ptr, inst);
  printf("Bound info - Base: %p, Bound: %p\n", metadata.base, metadata.bound);

  if (access >= metadata.base && access < metadata.bound)
  {
    printf("Access within bounds.\n");
  }
  else
  {
    printf("*** Out-of-bound access detected ***\n");
  }
}

void initialize_metadata_table()
{
  primary_table = mmap(NULL, sizeof(Metadata *) * PRIMARY_TABLE_SIZE,
                       PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (primary_table == MAP_FAILED)
  {
    perror("mmap failed");
    exit(1);
  }
  for (size_t i = 0; i < PRIMARY_TABLE_SIZE; i++)
  {
    primary_table[i] = NULL; // 초기화 시 2차 테이블은 NULL로 설정
  }
  printf("Primary table initialization done\n");
}
void print_metadata(void *base, void *bound){
  printf("base address: %p\n", base);
  printf("bound address: %p\n", bound);
}

/*
size_t hash(void *ptr)
{
  return ((uintptr_t)ptr >> 4) % HASH_TABLE_SIZE;
}

// 메타데이터 설정 함수
void set_metadata(void *ptr, size_t size)
{
  size_t index = hash(ptr);
  metadata_table[index].base = ptr;
  metadata_table[index].bound = (char *)ptr + size;
  printf("Stored Malloc Info - base: %p, bound: %p\n", metadata_table[index].base, metadata_table[index].bound);
}

bool get_metadata(void *ptr)
{
  size_t index = hash(ptr);
  Metadata metadata = metadata_table[index];

  return (metadata.base != NULL && metadata.bound != NULL);
}

void print_metadata_table()
{
  printf("Printing non-empty entries in metadata table:\n");
  for (size_t i = 0; i < HASH_TABLE_SIZE; i++)
  {
    if (metadata_table[i].base != NULL || metadata_table[i].bound != NULL)
    {
      printf("Index %zu -> Base: %p, Bound: %p\n", i, metadata_table[i].base, metadata_table[i].bound);
    }
  }
}

void bound_check(void *ptr, void *access, const char *inst)
{
  size_t index = hash(ptr);
  Metadata metadata = metadata_table[index];

  // 경계 정보가 제대로 설정되어 있는지 확인
  if (metadata.base == NULL || metadata.bound == NULL)
  {
    printf("Error: Metadata not found for pointer %p\n", ptr);
    return;
  }

  printf("Memory access at %p for pointer %p[%s]\n", access, ptr, inst);
  printf("Bound info - Base: %p, Bound: %p\n", metadata.base, metadata.bound);

  // 접근 주소가 base 이상이고, bound보다 작은지 확인
  if (access >= metadata.base && access < metadata.bound)
  {
    printf("Access within bounds.\n");
  }
  else
  {
    printf("*** Out-of-bound access detected ***\n");
  }
}

void initialize_metadata_table()
{
  size_t mmap_size = sizeof(Metadata) * HASH_TABLE_SIZE;

  metadata_table = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (metadata_table == MAP_FAILED)
  {
    perror("mmap failed");
    exit(1);
  }
  else
  {
    printf("mmap done\n");
  }

  for (size_t i = 0; i < HASH_TABLE_SIZE; i++)
  {
    metadata_table[i].base = NULL;
    metadata_table[i].bound = NULL;
  }
}
*/