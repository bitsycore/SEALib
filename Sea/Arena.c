#include "Arena.h"

#include <stdint.h>
#include <stdalign.h>
#include "Allocator.h"

_Thread_local int ArenaErrno = 0;

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
} max_align_custom_t;

static void init(struct Arena* self, void* buffer, size_t capacity) {
	self->buffer = (uint8_t*) buffer;
	self->capacity = capacity;
	self->offset = 0;
}

static void* allocEx(struct Arena* self, size_t size, size_t alignment) {
	if (self == NULL) {
		ArenaErrno = ARENA_ERROR_INVALID_CONTEXT;
		return NULL;
	}

	// Alignment must be a power of 2
	if (alignment & (alignment - 1)) {
		ArenaErrno = ARENA_ERROR_INVALID_ALIGNMENT;
		return NULL;
	}

	size_t current = (size_t) (self->buffer + self->offset);
	size_t aligned = (current + alignment - 1) & ~(alignment - 1);
	size_t next_offset = aligned - (size_t) self->buffer + size;

	if (next_offset > self->capacity) {
		ArenaErrno = ARENA_ERROR_OUT_OF_MEMORY;
		return NULL;
	}

	void* ptr = (void*) (self->buffer + (aligned - (size_t) self->buffer));
	self->offset = next_offset;
	return ptr;
}

static void* alloc(struct Arena* self, size_t size) {
	return allocEx(self, size, alignof(max_align_custom_t));
}

static void reset(struct Arena* self) {
	self->offset = 0;
}

static size_t remaining(struct Arena* self) {
	return self->capacity - self->offset;
}

static struct Allocator getAllocator(struct Arena* self) {
	return (struct Allocator) {
		.alloc = (void* (*)(void*, size_t, size_t)) allocEx,
		.context = self
	};
}

static int* getErrno(void) {
	return &ArenaErrno;
}

const struct Arena_CLS Arena = {
	.getErrno = getErrno,
	.init = init,
	.alloc = alloc,
	.allocEx = allocEx,
	.reset = reset,
	.remaining = remaining,
	.getAllocator = getAllocator,
};
