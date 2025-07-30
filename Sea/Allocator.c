#include "Allocator.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// =========================================
// MARK: Custom Allocator Utility
// =========================================

static void* malloc_allocator_func(void* _, const size_t size, const size_t alignment) {
	(void)alignment;
	return malloc(size);
}

static void free_allocator_func(void* _, void* ptr, const size_t size) {
	(void)size;
	free(ptr);
}

static struct SeaAllocator MALLOC_ALLOCATOR = {
	.context = NULL,
	.alloc = malloc_allocator_func,
	.free = free_allocator_func,
};

// =========================================
// MARK: Custom Allocator Utility
// =========================================


static char* alloc_strdup(struct SeaAllocator* self, const char* str) {
	if (!str || !self || !self->alloc) return NULL;

	const size_t len = strlen(str);
	char* result = self->alloc(self->context, len + 1, 1);
	if (!result) return NULL;

	strcpy(result, str);
	return result;
}

const struct SeaAllocator_CLS SeaAllocator = {
	.strdup = alloc_strdup,
	.Malloc = &MALLOC_ALLOCATOR,
};