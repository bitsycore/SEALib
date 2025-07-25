#include "Arena.h"
#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>

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
} max_align_t_custom;

static void init(struct Arena* self, void* buffer, size_t capacity) {
    self->buffer = (uint8_t*)buffer;
    self->capacity = capacity;
    self->offset = 0;
}

static void* alloc(struct Arena* self, size_t size, size_t alignment) {
    if (alignment == 0)
        alignment = alignof(max_align_t_custom);

    size_t current = (size_t)(self->buffer + self->offset);
    size_t aligned = (current + alignment - 1) & ~(alignment - 1);
    size_t next_offset = aligned - (size_t)self->buffer + size;

    if (next_offset > self->capacity)
        return NULL;

    void* ptr = (void*)(self->buffer + (aligned - (size_t)self->buffer));
    self->offset = next_offset;
    return ptr;
}

static void reset(struct Arena* self) {
    self->offset = 0;
}

static size_t remaining(struct Arena* self) {
    return self->capacity - self->offset;
}

const struct Arena_CLS Arena = {
        .init = init,
        .alloc = alloc,
        .reset = reset,
        .remaining = remaining
};
