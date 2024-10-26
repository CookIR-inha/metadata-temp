// softbound.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <bits/mman-linux.h>

#define HASH_TABLE_SIZE (1 << 20) // 2^20

typedef struct
{
  void *base;
  void *bound;
} Metadata;

// Metadata metadata_table[HASH_TABLE_SIZE];
Metadata *metadata_table = NULL;

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
