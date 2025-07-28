#ifndef ARENA_SEALIB_3c01bdbb_H
#define ARENA_SEALIB_3c01bdbb_H

#include <stddef.h>
#include <stdint.h>
#include "Allocator.h"

enum SeaArenaErrors {
	ARENA_ERROR_OUT_OF_MEMORY = 1,
	ARENA_ERROR_INVALID_ALIGNMENT = 2,
	ARENA_ERROR_INVALID_CONTEXT = 3,
};

struct SeaArena {
	size_t capacity;
	size_t offset;
	uint8_t* buffer;
};

extern const struct SeaArena_CLS {
	void (* init)(struct SeaArena* self, void* buffer, size_t capacity);
	void* (* alloc)(struct SeaArena* self, size_t size);
	void* (* allocEx)(struct SeaArena* self, size_t size, size_t alignment);
	void (* reset)(struct SeaArena* self);
	size_t (* remaining)(const struct SeaArena* self);
	struct SeaAllocator (* getAllocator)(struct SeaArena* self);
} SeaArena;

#define SEA_ARENA_SCOPE(arena) \
    for (bool _done = false; _done == false; _done = true, SeaArena.reset(arena))

#endif //ARENA_SEALIB_3c01bdbb_H