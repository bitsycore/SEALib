#include "Allocator.h"

#include <stddef.h>
#include <stdlib.h>

static void* malloc_allocator_func(void* _, const size_t size, const size_t alignment) {
	(void)alignment;
	return malloc(size);
}

static struct SeaAllocator MALLOC_ALLOCATOR = {
	.context = NULL,
	.alloc = malloc_allocator_func,
};

const struct SeaAllocator_CLS SeaAllocator = {
	.malloc = &MALLOC_ALLOCATOR,
};