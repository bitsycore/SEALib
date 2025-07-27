#ifndef ALLOCATOR_SEALIB_3c01bdbb_H
#define ALLOCATOR_SEALIB_3c01bdbb_H

#include <stddef.h>
#include <stdint.h>

struct Allocator {
	void* (* alloc)(void* ctx, size_t size, size_t alignment);
	void* context;
};

extern const struct Allocator_CLS {
	struct Allocator* malloc;
} Allocator;

#endif // ALLOCATOR_SEALIB_3c01bdbb_H