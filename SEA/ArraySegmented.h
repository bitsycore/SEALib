#ifndef SEALIB_ARRAY_H
#define SEALIB_ARRAY_H

#include <stddef.h>
#include <stdint.h>

#define SEA_SEGMENT_ARRAY_SEGMENTS_TO_SKIP 6

struct SEA_ArraySegmented {
	struct SEA_Allocator* allocator;
	size_t elementSize;
	uint32_t count;
	size_t usedSegments;
	uint8_t* segments[26];
};

#define SEA_ArraySegmented(type, _allocator) \
	(struct SEA_ArraySegmented) { \
		.allocator = (_allocator == NULL ? SEA_Allocator.Heap : _allocator), \
		.elementSize = (sizeof(type)), \
		.count = 0,\
		.usedSegments = 0, \
		.segments = {0}, \
	}

#define SEA_ArraySegmented_foreach(type, sa) \
for ( \
	struct { \
		size_t index; \
		type* value; \
	} it = { \
		0, \
		(SEA_ArraySegmented.count(sa) == 0) ? NULL : (type*)SEA_ArraySegmented.get(sa, 0) \
	}; \
	it.value != NULL && it.index < SEA_ArraySegmented.count(sa); \
	it.index++, it.value = (type*)SEA_ArraySegmented.get(sa, it.index) \
)

extern const struct SEA_ArraySegmented_CLS {
	void* (*get)(const struct SEA_ArraySegmented* sa, uint32_t index);
	void* (*alloc)(struct SEA_ArraySegmented* sa);
	size_t (*add)(struct SEA_ArraySegmented* sa, const void* ptr);
	size_t (*count)(const struct SEA_ArraySegmented* sa);
	void (*free)(struct SEA_ArraySegmented* sa);
} SEA_ArraySegmented;

#endif //SEALIB_ARRAY_H