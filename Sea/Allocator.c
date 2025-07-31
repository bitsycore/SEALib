#include "Allocator.h"

#include <stdlib.h>
#include <string.h>

#include "Compat.h"

// =========================================
// MARK: Custom Allocator Utility
// =========================================

static void* Func_HeapAlloc(void* _, const size_t size) {
	return malloc(size);
}

static void* Func_HeapAllocEx(void* _, const size_t size, const size_t alignment) {
	(void)alignment;
	return malloc(size);
}

static void Func_HeapFree(void* _, void* ptr) {
	free(ptr);
}

static struct SeaAllocator HEAP_ALLOCATOR = {
	.context = NULL,
	.alloc = Func_HeapAlloc,
	.allocEx = Func_HeapAllocEx,
	.free = Func_HeapFree,
};

// =========================================
// MARK: Custom Allocator Utility
// =========================================

static char* Strdup(struct SeaAllocator* self, const char* str, const size_t len) {
	if (!str || !self || !self->alloc) return NULL;

	const size_t vLen = len == 0 ? strlen(str) + 1 :  len;
	char* result = self->allocEx(self->context, vLen, SEA_ALIGNOF(char));
	if (!result) return NULL;
#ifdef _MSC_VER
	strcpy_s(result, vLen , str);
#else
	strcpy(result, str);
#endif
	return result;
}

static void* Alloc(const struct SeaAllocator* self, const size_t size) {
	if (!self || !self->alloc || size == 0) return NULL;
	return self->alloc(self->context, size);
}

static void* AllocEx(const struct SeaAllocator* self, const size_t size, const size_t alignment) {
	if (!self || !self->allocEx || size == 0) return NULL;
	return self->allocEx(self->context, size, alignment);
}

static void Free(const struct SeaAllocator* self, void* ptr) {
	if (!self || !self->free || !ptr) return;
	self->free(self->context, ptr);
}

const struct SeaAllocator_CLS SeaAllocator = {
	.strdup = Strdup,
	.alloc = Alloc,
	.allocEx = AllocEx,
	.free = Free,
	.Malloc = &HEAP_ALLOCATOR,
};