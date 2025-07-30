#include "Arena.h"

#include "Allocator.h"
#include "Error.h"
#include "Compat.h"

#include <stdint.h>

typedef union {
	char c;
	short s;
	int i;
	long l;
	long long ll;
	float f;
	double d;
	long double ld;
	void* p;
} SEA_Max_Align;

static void init(struct SeaArena* self, void* buffer, const size_t capacity) {
	self->buffer = (uint8_t*) buffer;
	self->capacity = capacity;
	self->offset = 0;
}

static void* allocEx(struct SeaArena* self, const size_t size, const size_t alignment) {
	if (self == NULL) {
		SeaError.SetError(SEA_ERROR_ARENA_INVALID_CONTEXT);
		return NULL;
	}

	int vAlignment = 0;
	if (alignment == 0) {
		vAlignment = SEA_ALIGNOF(SEA_Max_Align);
	} else {
		vAlignment = alignment;
	}

	// Alignment must be a power of 2
	if (vAlignment & (vAlignment - 1)) {
		SeaError.SetError(SEA_ERROR_ARENA_INVALID_ALIGNMENT);
		return NULL;
	}

	size_t current = (size_t) (self->buffer + self->offset);
	size_t aligned = (current + vAlignment - 1) & ~(vAlignment - 1);
	size_t next_offset = aligned - (size_t) self->buffer + size;

	if (next_offset > self->capacity) {
		SeaError.SetError(SEA_ERROR_ARENA_OUT_OF_MEMORY);
		return NULL;
	}

	void* ptr = self->buffer + (aligned - (size_t) self->buffer);
	self->offset = next_offset;
	return ptr;
}

static void* alloc(struct SeaArena* self, const size_t size) {
	return allocEx(self, size, 0);
}

static void reset(struct SeaArena* self) {
	self->offset = 0;
}

static size_t remaining(const struct SeaArena* self) {
	return self->capacity - self->offset;
}

static struct SeaAllocator getAllocator(struct SeaArena* self) {
	return (struct SeaAllocator) {
		.alloc = (void* (*)(void*, size_t, size_t)) allocEx,
		.context = self
	};
}

const struct SeaArena_CLS SeaArena = {
	.init = init,
	.alloc = alloc,
	.allocEx = allocEx,
	.reset = reset,
	.remaining = remaining,
	.getAllocator = getAllocator,
};