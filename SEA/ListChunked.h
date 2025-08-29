#ifndef SEALIB_ARRAY_CHUNKED_H
#define SEALIB_ARRAY_CHUNKED_H

#include "Allocator.h"

#include <stddef.h>

// =======================================
// MARK: Config
// =======================================

#define SEA_CHUNKED_ARRAY_CHUNK_SIZE 64

// =======================================
// MARK: Types
// =======================================

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

// =======================================
// MARK: Macro
// =======================================

#define SEA_ListChunked(_type_, _allocator_) \
    (struct SEA_ListChunked) { \
        .allocator = (_allocator_ == NULL ? SEA_Allocator_Heap : _allocator_), \
        .head = NULL, \
        .tail = NULL, \
        .elementSize = sizeof(_type_), \
        .totalCount = 0, \
        .chunkCapacity = SEA_CHUNKED_ARRAY_CHUNK_SIZE, \
    }

#define SEA_ListChunked_foreach(_type_, _list_) \
    for ( \
        struct { \
            size_t index; \
            _type_* value; \
        } it = { \
            0, \
            (SEA_ListChunked.count(_list_) == 0) ? NULL : (_type_*)SEA_ListChunked.get(_list_, 0) \
        }; \
        it.value != NULL && it.index < SEA_ListChunked.count(_list_); \
        it.index++, it.value = (_type_*)SEA_ListChunked.get(_list_, it.index) \
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
