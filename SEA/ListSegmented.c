#include "ListSegmented.h"

#include <stdint.h>
#include <string.h>

#include "Allocator.h"
#include "Compat/MathCompat.h"

static uint32_t capacityForSegmentCount(const int segment_count) {
	return ((1 << SEA_SEGMENT_ARRAY_SEGMENTS_TO_SKIP) << segment_count) - (1 << SEA_SEGMENT_ARRAY_SEGMENTS_TO_SKIP);
}

static void* ListSegmented_get(const struct SEA_ListSegmented* sa, const uint32_t index) {
	if (index >= sa->count) {
		return NULL;
	}
	const int segment = SEA_log2i((index >> SEA_SEGMENT_ARRAY_SEGMENTS_TO_SKIP) + 1);
	const uint32_t slot = index - capacityForSegmentCount(segment);
	return (char *)sa->segments[segment] + sa->elementSize * slot;
}


static void* ListSegmented_alloc(struct SEA_ListSegmented* sa) {
	if (sa->count >= capacityForSegmentCount(sa->usedSegments)) {
		const size_t slots_in_segment = (1 << SEA_SEGMENT_ARRAY_SEGMENTS_TO_SKIP) << sa->usedSegments;
		const size_t segment_size = sa->elementSize * slots_in_segment;
		sa->segments[sa->usedSegments] = SEA_Allocator_alloc(sa->allocator, segment_size);
		sa->usedSegments++;
	}
	sa->count++;
	return ListSegmented_get(sa, sa->count - 1);
}

static size_t ListSegmented_add(struct SEA_ListSegmented* sa, const void* ptr) {
	memcpy(ListSegmented_alloc(sa), ptr, sa->elementSize);
	return sa->count;
}

static size_t ListSegmented_count(const struct SEA_ListSegmented* sa) {
	return sa->count;
}

static void ListSegmented_free(struct SEA_ListSegmented* sa) {
	for (int i = 0; i < sa->usedSegments; i++) {
		SEA_Allocator_free(sa->allocator, sa->segments[i]);
	}
	sa->count = 0;
	sa->elementSize = 0;
}

const struct SEA_ListSegmented_CLS SEA_ListSegmented = {
	.get = ListSegmented_get,
	.alloc = ListSegmented_alloc,
	.add = ListSegmented_add,
	.count = ListSegmented_count,
	.free = ListSegmented_free,
};