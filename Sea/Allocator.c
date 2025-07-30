#include "Allocator.h"

#include <stdlib.h>
#include <string.h>

// =========================================
// MARK: Custom Allocator Utility
// =========================================

static void* Func_HeapAlloc(void* _, const size_t size, const size_t alignment) {
	(void)alignment;
	return malloc(size);
}

static void Func_HeapFree(void* _, void* ptr) {
	free(ptr);
}

static struct SeaAllocator HEAP_ALLOCATOR = {
	.context = NULL,
	.alloc = Func_HeapAlloc,
	.free = Func_HeapFree,
};

// =========================================
// MARK: Custom Allocator Utility
// =========================================

static char* Strdup(struct SeaAllocator* self, const char* str) {
	if (!str || !self || !self->alloc) return NULL;

	const size_t len = strlen(str);
	char* result = self->alloc(self->context, len + 1, 1);
	if (!result) return NULL;

	strcpy(result, str);
	return result;
}

const struct SeaAllocator_CLS SeaAllocator = {
	.strdup = Strdup,
	.Malloc = &HEAP_ALLOCATOR,
};