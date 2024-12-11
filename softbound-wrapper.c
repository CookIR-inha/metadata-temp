#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "softbound.h"
#include "shadow_memory.h"

void *_softboundcets_malloc(size_t size)
{
    char *ret_addr = (char *)malloc(size);
    char *ret_bound = ret_addr + size;
    //Softboundcets
    __softboundcets_store_base_shadow_stack(ret_addr, 0);
    __softboundcets_store_bound_shadow_stack(ret_bound, 0);
    //ASan
    set_shadow_enc_alloc(ret_addr,size);
    return ret_addr;
}
void *_softboundcets_free(void *addr){
    int *value = (int *)((uintptr_t)addr - 0x8);
	size_t size = *value & ~0x7;
    set_shadow_enc_free(addr, size);
}
char *_softboundcets_strncpy(char *dest, char *src, size_t n) {

  char *dest_base = (char *)__softboundcets_load_base_shadow_stack(1);
  char *dest_bound = (char *)__softboundcets_load_bound_shadow_stack(1);

  char *src_base = (char *)__softboundcets_load_base_shadow_stack(2);
  char *src_bound = (char *)__softboundcets_load_bound_shadow_stack(2);

  /* Can either (dest + n) or (src + n) overflow? */
  if (dest < dest_base || (dest + n > dest_bound)) {
    printf("[strncpy] overflow in strncpy with dest\n");
    abort();
  }
  if (src < src_base || (src > src_bound - n)) {
    abort();
  }

  char *ret_ptr = strncpy(dest, src, n);
  __softboundcets_propagate_metadata_shadow_stack_from(1, 0);
  return ret_ptr;
}
char *_softboundcets_strcpy(char *dest, char *src) {

  char *dest_base = (char *)__softboundcets_load_base_shadow_stack(1);
  char *dest_bound = (char *)__softboundcets_load_bound_shadow_stack(1);

  char *src_base = (char *)__softboundcets_load_base_shadow_stack(2);
  char *src_bound = (char *)__softboundcets_load_bound_shadow_stack(2);

  /* There will be an out-of-bound read before we trigger an error as
     we currently use strlen. Can either (dest + size) or (src + size)
     overflow?
  */
#ifndef __NOSIM_CHECKS
  size_t size = strlen(src);
  if (dest < dest_base || (dest > dest_bound - size - 1) ||
      (size > (size_t)dest_bound)) {
    printf("[strcpy] overflow in strcpy with dest\n");
    abort();
  }
  if (src < src_base || (src > src_bound - size - 1) ||
      (size > (size_t)src_bound)) {
    printf("[strcpy] overflow in strcpy with src\n");
   abort();
  }
#endif

  char *ret_ptr = strcpy(dest, src);
  __softboundcets_propagate_metadata_shadow_stack_from(1, 0);
  return ret_ptr;
}
