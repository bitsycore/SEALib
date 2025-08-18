
#ifndef SEALIB_ARRAY_DYNAMIC_H
#define SEALIB_ARRAY_DYNAMIC_H

#include <stddef.h>
#include <stdint.h>
#include "Allocator.h"
#include "Iterator.h"

#define SEA_DYNAMIC_ARRAY_INITIAL_CAPACITY 16
#define SEA_DYNAMIC_ARRAY_GROWTH_FACTOR 2

struct SEA_ListDyn {
    struct SEA_Allocator* allocator;
    void* data;
    size_t elementSize;
    size_t count;
    size_t capacity;
};

#define SEA_ListDyn(type, _allocator) \
    (struct SEA_ListDyn) { \
        .allocator = (_allocator == NULL ? SEA_Allocator.Heap : _allocator), \
        .data = NULL, \
        .elementSize = sizeof(type), \
        .count = 0, \
        .capacity = 0, \
    }

#define SEA_ListDyn_foreach(type, da) \
    for ( \
        struct { \
            size_t index; \
            type* value; \
        } it = { \
            0, \
            (SEA_ListDyn.count(da) == 0) ? NULL : (type*)SEA_ListDyn.get(da, 0) \
        }; \
        it.value != NULL && it.index < SEA_ListDyn.count(da); \
        it.index++, it.value = (type*)SEA_ListDyn.get(da, it.index) \
    )

extern const struct SEA_ListDyn_CLS {
    void* (*get)(const struct SEA_ListDyn* da, size_t index);
    void* (*alloc)(struct SEA_ListDyn* da);
    size_t (*add)(struct SEA_ListDyn* da, const void* ptr);
    size_t (*insert)(struct SEA_ListDyn* da, size_t index, const void* ptr);
    void (*remove)(struct SEA_ListDyn* da, size_t index);
    size_t (*count)(const struct SEA_ListDyn* da);
    size_t (*capacity)(const struct SEA_ListDyn* da);
    void (*reserve)(struct SEA_ListDyn* da, size_t capacity);
    void (*shrink)(struct SEA_ListDyn* da);
    void (*clear)(struct SEA_ListDyn* da);
    void (*free)(struct SEA_ListDyn* da);
    struct SEA_Iterator* (*iterator)(struct SEA_ListDyn* da, const struct SEA_Allocator* allocatorOverride);
} SEA_ListDyn;

#endif //SEALIB_ARRAY_DYNAMIC_H
