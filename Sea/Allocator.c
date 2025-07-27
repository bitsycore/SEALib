#include "Allocator.h"

#include <stdlib.h>
#include <stddef.h>

static void* allocator_malloc_alloc(void* ctx, size_t size, size_t alignment) {
	return malloc(size);
}

static struct Allocator allocator_malloc = {
	.context = NULL,
	.alloc = allocator_malloc_alloc,
};

const struct Allocator_CLS Allocator = {
	.malloc = &allocator_malloc,
};