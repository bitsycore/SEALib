#include "Allocator.h"

#include <stdlib.h>
#include <string.h>

#include "Align.h"

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

static struct SEA_Allocator HEAP_ALLOCATOR = {
	.context = NULL,
	.alloc = Func_HeapAlloc,
	.allocAligned = Func_HeapAllocEx,
	.free = Func_HeapFree,
};

// =========================================
// MARK: Custom Allocator Utility
// =========================================

static char* Allocator_Strdup(struct SEA_Allocator* self, const char* str, const size_t len) {
	if (!str || !self || !self->alloc) return NULL;

	const size_t vLen = len == 0 ? strlen(str) + 1 :  len;
	char* result = self->allocAligned(self->context, vLen, SEA_alignof(char));
	if (!result) return NULL;
#ifdef _MSC_VER
	strcpy_s(result, vLen , str);
#else
	strcpy(result, str);
#endif
	return result;
}

static void* Allocator_Alloc(const struct SEA_Allocator* self, const size_t size) {
	if (!self || !self->alloc || size == 0) return NULL;
	return self->alloc(self->context, size);
}

static void* Allocator_AllocAligned(const struct SEA_Allocator* self, const size_t size, const size_t alignment) {
	if (!self || !self->allocAligned || size == 0) return NULL;
	return self->allocAligned(self->context, size, alignment);
}

static void Allocator_Free(const struct SEA_Allocator* self, void* ptr) {
	if (!self || !self->free || !ptr) return;
	self->free(self->context, ptr);
}

const struct SEA_Allocator_CLS SEA_Allocator = {
	.strdup = Allocator_Strdup,
	.alloc = Allocator_Alloc,
	.allocAligned = Allocator_AllocAligned,
	.free = Allocator_Free,
	.Malloc = &HEAP_ALLOCATOR,
};