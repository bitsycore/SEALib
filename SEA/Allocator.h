#ifndef SEA_ALLOCATOR_H
#define SEA_ALLOCATOR_H

#include "Config/CompConfig.h"

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

// =======================================
// MARK: Alias
// =======================================

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

typedef SEA_Allocator Allocator;

extern Allocator* const Allocator_Heap __asm("SEA_Allocator_Heap");

static inline char* Allocator_strdup(Allocator* self, const char* str, const size_t len) { return SEA_Allocator_strdup(self, str, len); }
static inline void* Allocator_alloc(Allocator* self, const size_t size) { return SEA_Allocator_alloc(self, size); }
static inline void* Allocator_allocAligned(Allocator* self, const size_t size, const size_t alignment) { return SEA_Allocator_allocAligned(self, size, alignment); }
static inline void Allocator_free(Allocator* self, void* ptr) { SEA_Allocator_free(self, ptr); }

#endif

#endif //SEA_ALLOCATOR_H
