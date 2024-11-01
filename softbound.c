// softbound.c
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>

#define HASH_TABLE_SIZE (1 << 20) // 2^20

typedef struct
{
  void *base;
  void *bound;
} Metadata;

Metadata metadata_table[HASH_TABLE_SIZE];
// Metadata *metadata_table = NULL;

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

void bound_check(void *ptr, void *access)
{
  printf("memory access at %p\n", access);
  size_t index = hash(ptr);
  void *bound = metadata_table[index].bound;
  printf("bound info: %p\n",bound);
  if(bound < access ){
    printf("*** out-of-bound access ***\n");
  }
}
void print_metadata(void *base, void *bound){
  printf("base address: %p\n", base);
  printf("bound address: %p\n", bound);
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
