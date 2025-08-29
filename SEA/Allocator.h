#ifndef SEA_ALLOCATOR_H
#define SEA_ALLOCATOR_H

#include <stddef.h>

// =======================================
// MARK: Types
// =======================================

typedef struct SEA_Allocator {
	void* (*alloc)(void* ctx, size_t size);
	void* (*allocAligned)(void* ctx, size_t size, size_t alignment);
	void (*free)(void* ctx, void* ptr);
	void* context;
} SEA_Allocator;

// =======================================
// MARK: Static
// =======================================

extern SEA_Allocator* const SEA_Allocator_Heap;

// =======================================
// MARK: Instance
// =======================================

char* SEA_Allocator_strdup(SEA_Allocator* self, const char* str, size_t len);
void* SEA_Allocator_alloc(SEA_Allocator* self, size_t size);
void* SEA_Allocator_allocAligned(SEA_Allocator* self, size_t size, size_t alignment);
void SEA_Allocator_free(SEA_Allocator* self, void* ptr);

#endif //SEA_ALLOCATOR_H
