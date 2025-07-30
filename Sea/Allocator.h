#ifndef ALLOCATOR_SEALIB_3c01bdbb_H
#define ALLOCATOR_SEALIB_3c01bdbb_H

#include <stddef.h>
#include <string.h>

struct SeaAllocator {
	void* (*alloc)(void* ctx, size_t size, size_t alignment);
	void (*free)(void* ctx, void* ptr);
	void* context;
};

extern const struct SeaAllocator_CLS {
	char* (*strdup)(struct SeaAllocator* self, const char* str);
	struct SeaAllocator* Malloc;
} SeaAllocator;

#endif // ALLOCATOR_SEALIB_3c01bdbb_H