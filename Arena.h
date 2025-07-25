#ifndef ARENA_SEA_3c01bdbb_H
#define ARENA_SEA_3c01bdbb_H

#include <stddef.h>
#include <stdint.h>

typedef void* (*allocator_t)(void* arena, size_t size, size_t alignment);

struct Arena {
    uint8_t* buffer;
    size_t capacity;
    size_t offset;
};

extern const struct Arena_CLS {
    void (*init)(struct Arena* self, void* buffer, size_t capacity);
    void* (*alloc)(struct Arena* self, size_t size, size_t alignment);
    void (*reset)(struct Arena* self);
    size_t (*remaining)(struct Arena* self);
} Arena;

#endif //ARENA_SEA_3c01bdbb_H
