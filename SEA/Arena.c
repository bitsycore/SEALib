#include "Arena.h"

#include "Allocator.h"
#include "Error.h"

#include <stdint.h>

#include "Compat/AlignCompat.h"

void SEA_Arena_init(SEA_Arena* self, void* buffer, const size_t capacity) {
	self->buffer = (uint8_t*) buffer;
	self->capacity = capacity;
	self->offset = 0;
}

void* SEA_Arena_allocAligned(SEA_Arena* self, const size_t size, const size_t alignment) {
	if (self == NULL) {
		SEA_Error_SetError(SEA_ERROR_GENERIC_ARGUMENT_NULL);
		return NULL;
	}

	const size_t vAlignment = alignment == 0 ? SEA_alignof(SEA_MaxAlign) : alignment;

	// Alignment must be a power of 2
	if (vAlignment & (vAlignment - 1)) {
		SEA_Error_SetError(SEA_ERROR_ARENA_INVALID_ALIGNMENT);
		return NULL;
	}

	const size_t current = (size_t) (self->buffer + self->offset);
	const size_t aligned = (current + vAlignment - 1) & ~(vAlignment - 1);
	const size_t next_offset = aligned - (size_t) self->buffer + size;

	if (next_offset > self->capacity) {
		SEA_Error_SetError(SEA_ERROR_ARENA_OUT_OF_MEMORY);
		return NULL;
	}

	void* ptr = self->buffer + (aligned - (size_t) self->buffer);
	self->offset = next_offset;
	return ptr;
}

void* SEA_Arena_alloc(SEA_Arena* self, const size_t size) {
	return SEA_Arena_allocAligned(self, size, 0);
}

void SEA_Arena_reset(SEA_Arena* self) {
	self->offset = 0;
}

size_t SEA_Arena_remaining(const SEA_Arena* self) {
	return self->capacity - self->offset;
}

SEA_Allocator SEA_Arena_allocator(SEA_Arena* self) {
	return ( SEA_Allocator) {
		.alloc = (void* (*)(void*, size_t)) SEA_Arena_alloc,
		.allocAligned = (void* (*)(void*, size_t, size_t)) SEA_Arena_allocAligned,
		.free = NULL,
		.context = self
	};
}