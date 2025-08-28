#ifndef SEALIB_ARRAY_H
#define SEALIB_ARRAY_H

#include "Config/CompConfig.h"

#include <stddef.h>
#include <stdint.h>

#define SEA_SEGMENT_ARRAY_SEGMENTS_TO_SKIP 6

struct SEA_ListSegmented {
	struct SEA_Allocator* allocator;
	size_t elementSize;
	uint32_t count;
	size_t usedSegments;
	uint8_t* segments[26];
};

#define SEA_ListSegmented(type, _allocator) \
	(struct SEA_ListSegmented) { \
		.allocator = (_allocator == NULL ? SEA_Allocator_Heap : _allocator), \
		.elementSize = (sizeof(type)), \
		.count = 0,\
		.usedSegments = 0, \
		.segments = {0}, \
	}

#define SEA_ListSegmented_foreach(type, sa) \
for ( \
	struct { \
		size_t index; \
		type* value; \
	} it = { \
		0, \
		(SEA_ListSegmented.count(sa) == 0) ? NULL : (type*)SEA_ListSegmented.get(sa, 0) \
	}; \
	it.value != NULL && it.index < SEA_ListSegmented.count(sa); \
	it.index++, it.value = (type*)SEA_ListSegmented.get(sa, it.index) \
)

extern const struct SEA_ListSegmented_CLS {
	void* (*get)(const struct SEA_ListSegmented* sa, uint32_t index);
	void* (*alloc)(struct SEA_ListSegmented* sa);
	size_t (*add)(struct SEA_ListSegmented* sa, const void* ptr);
	size_t (*count)(const struct SEA_ListSegmented* sa);
	void (*free)(struct SEA_ListSegmented* sa);
} SEA_ListSegmented;

#endif //SEALIB_ARRAY_H
