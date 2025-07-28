#ifndef ALLOCATOR_SEALIB_3c01bdbb_H
#define ALLOCATOR_SEALIB_3c01bdbb_H

#include <stddef.h>

struct SeaAllocator {
	void* (* alloc)(void* ctx, size_t size, size_t alignment);
	void* context;
};

extern const struct SeaAllocator_CLS {
	struct SeaAllocator* malloc;
} SeaAllocator;

#endif // ALLOCATOR_SEALIB_3c01bdbb_H