#ifndef ARENA_SEALIB_3c01bdbb_H
#define ARENA_SEALIB_3c01bdbb_H

#include <stdint.h>
#include "Allocator.h"

struct SEA_Arena {
	size_t capacity;
	size_t offset;
	uint8_t* buffer;
};

extern const struct SEA_Arena_CLS {
	void (* init)(struct SEA_Arena* self, void* buffer, size_t capacity);
	void* (* alloc)(struct SEA_Arena* self, size_t size);
	void* (* allocEx)(struct SEA_Arena* self, size_t size, size_t alignment);
	void (* reset)(struct SEA_Arena* self);
	size_t (* remaining)(const struct SEA_Arena* self);
	struct SEA_Allocator (* allocator)(struct SEA_Arena* self);
} SEA_Arena;

#endif //ARENA_SEALIB_3c01bdbb_H