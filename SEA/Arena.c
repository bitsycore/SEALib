#include "Arena.h"

#include "Allocator.h"
#include "Error.h"

#include <stdint.h>

#include "Align.h"

static void init(struct SEA_Arena* self, void* buffer, const size_t capacity) {
	self->buffer = (uint8_t*) buffer;
	self->capacity = capacity;
	self->offset = 0;
}

static void* allocEx(struct SEA_Arena* self, const size_t size, const size_t alignment) {
	if (self == NULL) {
		SEA_Error.SetError(SEA_ERROR_ARENA_INVALID_CONTEXT);
		return NULL;
	}

	size_t vAlignment = 0;
	if (alignment == 0) {
		vAlignment = SEA_alignof(SEA_MaxAlign);
	} else {
		vAlignment = alignment;
	}

	// Alignment must be a power of 2
	if (vAlignment & (vAlignment - 1)) {
		SEA_Error.SetError(SEA_ERROR_ARENA_INVALID_ALIGNMENT);
		return NULL;
	}

	const size_t current = (size_t) (self->buffer + self->offset);
	const size_t aligned = (current + vAlignment - 1) & ~(vAlignment - 1);
	const size_t next_offset = aligned - (size_t) self->buffer + size;

	if (next_offset > self->capacity) {
		SEA_Error.SetError(SEA_ERROR_ARENA_OUT_OF_MEMORY);
		return NULL;
	}

	void* ptr = self->buffer + (aligned - (size_t) self->buffer);
	self->offset = next_offset;
	return ptr;
}

static void* alloc(struct SEA_Arena* self, const size_t size) {
	return allocEx(self, size, 0);
}

static void reset(struct SEA_Arena* self) {
	self->offset = 0;
}

static size_t remaining(const struct SEA_Arena* self) {
	return self->capacity - self->offset;
}

static struct SEA_Allocator getAllocator(struct SEA_Arena* self) {
	return (struct SEA_Allocator) {
		.alloc = (void* (*)(void*, size_t)) alloc,
		.allocEx = (void* (*)(void*, size_t, size_t)) allocEx,
		.free = NULL,
		.context = self
	};
}

const struct SEA_Arena_CLS SEA_Arena = {
	.init = init,
	.alloc = alloc,
	.allocEx = allocEx,
	.reset = reset,
	.remaining = remaining,
	.getAllocator = getAllocator,
};