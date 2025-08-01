#ifndef ALLOCATOR_SEALIB_3c01bdbb_H
#define ALLOCATOR_SEALIB_3c01bdbb_H

#include <stddef.h>
#include <string.h>

struct SEA_Allocator {
	void* (*alloc)(void* ctx, size_t size);
	void* (*allocEx)(void* ctx, size_t size, size_t alignment);
	void (*free)(void* ctx, void* ptr);
	void* context;
};

extern const struct SEA_Allocator_CLS {
	char* (*strdup)(struct SEA_Allocator* self, const char* str, size_t len);
	void* (*alloc)(const struct SEA_Allocator* self, size_t size);
	void* (*allocEx)(const struct SEA_Allocator* self, size_t size, size_t alignment);
	void (*free)(const struct SEA_Allocator* self, void* ptr);
	struct SEA_Allocator* Malloc;
} SEA_Allocator;

#endif // ALLOCATOR_SEALIB_3c01bdbb_H