#include "Allocator.h"

#include <stdlib.h>
#include <stddef.h>

static void* malloc_allocator_func(void* ctx, size_t size, size_t alignment) {
	return malloc(size);
}

static struct SeaAllocator MALLOC_ALLOCATOR = {
	.context = NULL,
	.alloc = malloc_allocator_func,
};

const struct SeaAllocator_CLS SeaAllocator = {
	.malloc = &MALLOC_ALLOCATOR,
};