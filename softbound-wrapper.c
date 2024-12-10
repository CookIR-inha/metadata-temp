#include <stdlib.h>
#include <stdio.h>

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
    printf("custom malloc called\n");
    return ret_addr;
}
void *_softboundcets_free(void *addr){
    int *value = (int *)((uintptr_t)addr - 0x8);
	size_t size = *value & ~0x7;
    set_shadow_enc_free(addr, size);
}