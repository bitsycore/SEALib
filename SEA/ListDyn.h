#ifndef SEALIB_ARRAY_DYNAMIC_H
#define SEALIB_ARRAY_DYNAMIC_H

#include "Config/CompConfig.h"

#include "Allocator.h"
#include "Iterator.h"

#include <stddef.h>

// =======================================
// MARK: Config
// =======================================

#define SEA_DYNAMIC_ARRAY_INITIAL_CAPACITY 16
#define SEA_DYNAMIC_ARRAY_GROWTH_FACTOR 2

// =======================================
// MARK: Types
// =======================================

typedef struct SEA_ListDyn {
    SEA_Allocator* allocator;
    void* data;
    size_t elementSize;
    size_t count;
    size_t capacity;
} SEA_ListDyn;

// =======================================
// MARK: Macro
// =======================================

#define SEA_ListDyn(type, _allocator) \
    (struct SEA_ListDyn) { \
        .allocator = (_allocator == NULL ? SEA_Allocator_Heap : _allocator), \
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
            (SEA_ListDyn_count(da) == 0) ? NULL : (type*)SEA_ListDyn_get(da, 0) \
        }; \
        it.value != NULL && it.index < SEA_ListDyn_count(da); \
        it.index++, it.value = (type*)SEA_ListDyn_get(da, it.index) \
    )

// =======================================
// MARK: Instance
// =======================================

void* SEA_ListDyn_get(const SEA_ListDyn* self, size_t index);
void* SEA_ListDyn_alloc(SEA_ListDyn* self);
size_t SEA_ListDyn_add(SEA_ListDyn* self, const void* ptr);
size_t SEA_ListDyn_insert(SEA_ListDyn* self, size_t index, const void* ptr);
void SEA_ListDyn_remove(SEA_ListDyn* self, size_t index);
size_t SEA_ListDyn_count(const SEA_ListDyn* self);
size_t SEA_ListDyn_capacity(const SEA_ListDyn* self);
void SEA_ListDyn_reserve(SEA_ListDyn* self, size_t capacity);
void SEA_ListDyn_shrink(SEA_ListDyn* self);
void SEA_ListDyn_clear(SEA_ListDyn* self);
void SEA_ListDyn_free(SEA_ListDyn* self);
SEA_Iterator* SEA_ListDyn_iterator(SEA_ListDyn* self, SEA_Allocator* allocatorOverride);

// =======================================
// MARK: Alias
// =======================================

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

// =========================
// Types
typedef SEA_ListDyn ListDyn;

// =========================
// Static
#define ListDyn(type, allocator) SEA_ListDyn(type, allocator)

// =========================
// Instance
#define ListDyn_foreach SEA_ListDyn_foreach
static inline void* ListDyn_get(const ListDyn* self, const size_t index) { return SEA_ListDyn_get(self, index); }
static inline void* ListDyn_alloc(ListDyn* self) { return SEA_ListDyn_alloc(self); }
static inline size_t ListDyn_add(ListDyn* self, const void* ptr) { return SEA_ListDyn_add(self, ptr); }
static inline size_t ListDyn_insert(ListDyn* self, const size_t index, const void* ptr) { return SEA_ListDyn_insert(self, index, ptr); }
static inline void ListDyn_remove(ListDyn* self, const size_t index) { SEA_ListDyn_remove(self, index); }
static inline size_t ListDyn_count(const ListDyn* self) { return SEA_ListDyn_count(self); }
static inline size_t ListDyn_capacity(const ListDyn* self) { return SEA_ListDyn_capacity(self); }
static inline void ListDyn_reserve(ListDyn* self, const size_t capacity) { SEA_ListDyn_reserve(self, capacity); }
static inline void ListDyn_shrink(ListDyn* self) { SEA_ListDyn_shrink(self); }
static inline void ListDyn_clear(ListDyn* self) { SEA_ListDyn_clear(self); }
static inline void ListDyn_free(ListDyn* self) { SEA_ListDyn_free(self); }
static inline SEA_Iterator* ListDyn_iterator(ListDyn* self, SEA_Allocator* allocatorOverride) { return SEA_ListDyn_iterator(self, allocatorOverride); }

#endif

#endif //SEALIB_ARRAY_DYNAMIC_H
