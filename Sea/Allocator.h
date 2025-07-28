#ifndef ALLOCATOR_SEALIB_3c01bdbb_H
#define ALLOCATOR_SEALIB_3c01bdbb_H

#include <stddef.h>
#include <string.h>

struct SeaAllocator {
	void* (*alloc)(void* ctx, size_t size, size_t alignment);
	void (*free)(void* ctx, void* ptr, size_t size);
	void* context;
};

extern const struct SeaAllocator_CLS {
	char* (*strdup)(struct SeaAllocator* self, const char* str);
	void (*free)(struct SeaAllocator* self, void* str);
	struct SeaAllocator* malloc;
} SeaAllocator;

#endif // ALLOCATOR_SEALIB_3c01bdbb_H