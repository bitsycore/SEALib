#include "Arena.h"

#include "Allocator.h"
#include "Error.h"

#include <stdint.h>

#include "Align.h"

static void Arena_init(struct SEA_Arena* self, void* buffer, const size_t capacity) {
	self->buffer = (uint8_t*) buffer;
	self->capacity = capacity;
	self->offset = 0;
}

static void* Arena_allocAligned(struct SEA_Arena* self, const size_t size, const size_t alignment) {
	if (self == NULL) {
		SEA_Error.SetError(SEA_ERROR_GENERIC_ARGUMENT_NULL);
		return NULL;
	}

	const size_t vAlignment = alignment == 0 ? SEA_alignof(SEA_MaxAlign) : alignment;

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

static void* Arena_alloc(struct SEA_Arena* self, const size_t size) {
	return Arena_allocAligned(self, size, 0);
}

static void Arena_reset(struct SEA_Arena* self) {
	self->offset = 0;
}

static size_t Arena_remaining(const struct SEA_Arena* self) {
	return self->capacity - self->offset;
}

static struct SEA_Allocator Arena_allocator(struct SEA_Arena* self) {
	return (struct SEA_Allocator) {
		.alloc = (void* (*)(void*, size_t)) Arena_alloc,
		.allocAligned = (void* (*)(void*, size_t, size_t)) Arena_allocAligned,
		.free = NULL,
		.context = self
	};
}

const struct SEA_Arena_CLS SEA_Arena = {
	.init = Arena_init,
	.alloc = Arena_alloc,
	.allocEx = Arena_allocAligned,
	.reset = Arena_reset,
	.remaining = Arena_remaining,
	.allocator = Arena_allocator,
};