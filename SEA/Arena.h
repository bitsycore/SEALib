#ifndef ARENA_SEALIB_3c01bdbb_H
#define ARENA_SEALIB_3c01bdbb_H

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

void SEA_Arena_init(SEA_Arena* self, void* buffer, size_t capacity);
void* SEA_Arena_alloc(SEA_Arena* self, size_t size);
void* SEA_Arena_allocEx(SEA_Arena* self, size_t size, size_t alignment);
void SEA_Arena_reset(SEA_Arena* self);
size_t SEA_Arena_remaining(const SEA_Arena* self);
SEA_Allocator SEA_Arena_allocator(SEA_Arena* self);

#endif //ARENA_SEALIB_3c01bdbb_H
