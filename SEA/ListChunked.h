#ifndef SEALIB_ARRAY_CHUNKED_H
#define SEALIB_ARRAY_CHUNKED_H

#include "Config/CompConfig.h"

#include "Allocator.h"

#include <stddef.h>

#define SEA_CHUNKED_ARRAY_CHUNK_SIZE 64

struct SEA_ListChunk {
    void* data;
    size_t count;
    struct SEA_ListChunk* next;
};

struct SEA_ListChunked {
    SEA_Allocator* allocator;
    struct SEA_ListChunk* head;
    struct SEA_ListChunk* tail;
    size_t elementSize;
    size_t totalCount;
    size_t chunkCapacity;
};

#define SEA_ListChunked(type, _allocator) \
    (struct SEA_ListChunked) { \
        .allocator = (_allocator == NULL ? SEA_Allocator_Heap : _allocator), \
        .head = NULL, \
        .tail = NULL, \
        .elementSize = sizeof(type), \
        .totalCount = 0, \
        .chunkCapacity = SEA_CHUNKED_ARRAY_CHUNK_SIZE, \
    }

#define SEA_ListChunked_foreach(type, ca) \
    for ( \
        struct { \
            size_t index; \
            type* value; \
        } it = { \
            0, \
            (SEA_ListChunked.count(ca) == 0) ? NULL : (type*)SEA_ListChunked.get(ca, 0) \
        }; \
        it.value != NULL && it.index < SEA_ListChunked.count(ca); \
        it.index++, it.value = (type*)SEA_ListChunked.get(ca, it.index) \
    )

extern const struct SEA_ListChunked_CLS {
    void* (*get)(const struct SEA_ListChunked* ca, size_t index);
    void* (*alloc)(struct SEA_ListChunked* ca);
    size_t (*add)(struct SEA_ListChunked* ca, const void* ptr);
    size_t (*count)(const struct SEA_ListChunked* ca);
    void (*clear)(struct SEA_ListChunked* ca);
    void (*free)(struct SEA_ListChunked* ca);
} SEA_ListChunked;

#endif //SEALIB_ARRAY_CHUNKED_H
