#include <stdlib.h>
#include <stdio.h>

#include "softbound.h"

void *_softboundcets_malloc(size_t size)
{
    char *ret_addr = (char *)malloc(size);
    char *ret_bound = ret_addr + size;
    __softboundcets_store_base_shadow_stack(ret_addr, 0);
    __softboundcets_store_bound_shadow_stack(ret_bound, 0);
    printf("custom malloc called\n");
    return ret_addr;
}