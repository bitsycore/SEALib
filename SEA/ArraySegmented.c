#include "ArraySegmented.h"

#include <stdint.h>
#include <string.h>

#include "Allocator.h"
#include "Compat/MathCompat.h"

static uint32_t capacityForSegmentCount(const int segment_count) {
	return ((1 << SEA_SEGMENT_ARRAY_SEGMENTS_TO_SKIP) << segment_count) - (1 << SEA_SEGMENT_ARRAY_SEGMENTS_TO_SKIP);
}

static void* ArraySegmented_get(const struct SEA_ArraySegmented* sa, const uint32_t index) {
	const int segment = SEA_log2i((index >> SEA_SEGMENT_ARRAY_SEGMENTS_TO_SKIP) + 1);
	const uint32_t slot = index - capacityForSegmentCount(segment);
	return (char *)sa->segments[segment] + sa->elementSize * slot;
}

static void* ArraySegmented_alloc(struct SEA_ArraySegmented* sa) {
	if (sa->count >= capacityForSegmentCount(sa->usedSegments)) {
		const size_t slots_in_segment = (1 << SEA_SEGMENT_ARRAY_SEGMENTS_TO_SKIP) << sa->usedSegments;
		const size_t segment_size = sa->elementSize * slots_in_segment;
		sa->segments[sa->usedSegments] = SEA_Allocator.alloc(sa->allocator, segment_size);
		sa->usedSegments++;
	}
	sa->count++;
	return ArraySegmented_get(sa, sa->count - 1);
}

static size_t ArraySegmented_add(struct SEA_ArraySegmented* sa, const void* ptr) {
	memcpy(ArraySegmented_alloc(sa), ptr, sa->elementSize);
	return sa->count;
}

static size_t ArraySegmented_count(const struct SEA_ArraySegmented* sa) {
	return sa->count;
}

static void ArraySegmented_free(struct SEA_ArraySegmented* sa) {
	for (int i = 0; i < sa->usedSegments; i++) {
		SEA_Allocator.free(sa->allocator, sa->segments[i]);
	}
	sa->count = 0;
	sa->elementSize = 0;
}

const struct SEA_ArraySegmented_CLS SEA_ArraySegmented = {
	.get = ArraySegmented_get,
	.alloc = ArraySegmented_alloc,
	.add = ArraySegmented_add,
	.count = ArraySegmented_count,
	.free = ArraySegmented_free,
};