#include "Allocator.h"

#include <stdlib.h>
#include <string.h>

#include "Compat/AlignCompat.h"
#include "Compat/StringCompat.h"

// =========================================
// MARK: Heap Allocator
// =========================================

static void* Func_HeapAlloc(void* _, const size_t size) {
	return malloc(size);
}

static void* Func_HeapAllocAligned(void* _, const size_t size, const size_t alignment) {
	(void)alignment;
	return malloc(size);
}

static void Func_HeapFree(void* _, void* ptr) {
	free(ptr);
}

static struct SEA_Allocator HEAP_ALLOCATOR = {
	.context = NULL,
	.alloc = Func_HeapAlloc,
	.allocAligned = Func_HeapAllocAligned,
	.free = Func_HeapFree,
};

struct SEA_Allocator* const SEA_Allocator_Heap = &HEAP_ALLOCATOR;

// =========================================
// MARK: Allocator
// =========================================

char* SEA_Allocator_strdup(struct SEA_Allocator* self, const char* str, const size_t len) {
	if (!str || !self || !self->alloc) return NULL;

	const size_t vLen = len == 0 ? strlen(str) + 1 :  len;
	char* result = self->allocAligned(self->context, vLen, SEA_alignof(char));
	if (!result) return NULL;
	SEA_strcpy_s(result, vLen , str);
	
	return result;
}

void* SEA_Allocator_alloc(struct SEA_Allocator* self, const size_t size) {
	if (!self || !self->alloc || size == 0) return NULL;
	return self->alloc(self->context, size);
}

void* SEA_Allocator_allocAligned(struct SEA_Allocator* self, const size_t size, const size_t alignment) {
	if (!self || !self->allocAligned || size == 0) return NULL;
	return self->allocAligned(self->context, size, alignment);
}

void SEA_Allocator_free(struct SEA_Allocator* self, void* ptr) {
	if (!self || !self->free || !ptr) return;
	self->free(self->context, ptr);
}