#ifndef ARENA_SEALIB_3c01bdbb_H
#define ARENA_SEALIB_3c01bdbb_H

#include "Config/CompConfig.h"
#include "Allocator.h"

#include <stdint.h>

// =======================================
// MARK: Types
// =======================================

typedef struct SEA_Arena {
	size_t capacity;
	size_t offset;
	uint8_t* buffer;
} SEA_Arena;

// =======================================
// MARK: Instance
// =======================================

void SEA_Arena_init(struct SEA_Arena* self, void* buffer, size_t capacity);
void* SEA_Arena_alloc(struct SEA_Arena* self, size_t size);
void* SEA_Arena_allocEx(struct SEA_Arena* self, size_t size, size_t alignment);
void SEA_Arena_reset(struct SEA_Arena* self);
size_t SEA_Arena_remaining(const struct SEA_Arena* self);
struct SEA_Allocator SEA_Arena_allocator(struct SEA_Arena* self);

// =======================================
// MARK: Alias
// =======================================

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

typedef SEA_Arena Arena;

static inline void Arena_init(Arena* self, void* buffer, size_t capacity) {
	SEA_Arena_init(self, buffer, capacity);
}

static inline void* Arena_alloc(Arena* self, size_t size) {
	return SEA_Arena_alloc(self, size);
}

static inline void* Arena_allocEx(Arena* self, size_t size, size_t alignment) {
	return SEA_Arena_allocEx(self, size, alignment);
}

static inline void Arena_reset(Arena* self) {
	SEA_Arena_reset(self);
}

static inline size_t Arena_remaining(const Arena* self) {
	return SEA_Arena_remaining(self);
}

static inline struct SEA_Allocator Arena_allocator(Arena* self) {
	return SEA_Arena_allocator(self);
}

#endif

#endif //ARENA_SEALIB_3c01bdbb_H