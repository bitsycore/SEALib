#ifndef ARENA_SEALIB_3c01bdbb_H
#define ARENA_SEALIB_3c01bdbb_H

#include <stddef.h>
#include <stdint.h>
#include "Allocator.h"

enum ArenaErrors {
	ARENA_ERROR_OUT_OF_MEMORY = 1,
	ARENA_ERROR_INVALID_ALIGNMENT = 2,
	ARENA_ERROR_INVALID_CONTEXT = 3,
};

struct Arena {
	size_t capacity;
	size_t offset;
	uint8_t* buffer;
};

extern const struct Arena_CLS {
	int* (* getErrno)(void);
	void (* init)(struct Arena* self, void* buffer, size_t capacity);
	void* (* alloc)(struct Arena* self, size_t size);
	void* (* allocEx)(struct Arena* self, size_t size, size_t alignment);
	void (* reset)(struct Arena* self);
	size_t (* remaining)(struct Arena* self);
	struct Allocator (* getAllocator)(struct Arena* self);
} Arena;

#endif //ARENA_SEALIB_3c01bdbb_H
