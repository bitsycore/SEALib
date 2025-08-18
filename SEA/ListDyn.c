#include "ListDyn.h"

#include <string.h>

static void ListDyn_ensureCapacity(struct SEA_ListDyn* da, const size_t minCapacity) {
    if (da->capacity >= minCapacity) return;
    
    size_t newCapacity = da->capacity == 0 ? SEA_DYNAMIC_ARRAY_INITIAL_CAPACITY : da->capacity;
    while (newCapacity < minCapacity) {
        newCapacity *= SEA_DYNAMIC_ARRAY_GROWTH_FACTOR;
    }
    
    void* newData = SEA_Allocator.alloc(da->allocator, newCapacity * da->elementSize);
    if (!newData) return;
    
    if (da->data) {
        memcpy(newData, da->data, da->count * da->elementSize);
        SEA_Allocator.free(da->allocator, da->data);
    }
    
    da->data = newData;
    da->capacity = newCapacity;
}

static void* ListDyn_get(const struct SEA_ListDyn* da, const size_t index) {
    if (index >= da->count) return NULL;
    return (char*)da->data + (index * da->elementSize);
}

static void* ListDyn_alloc(struct SEA_ListDyn* da) {
    ListDyn_ensureCapacity(da, da->count + 1);
    da->count++;
    return ListDyn_get(da, da->count - 1);
}

static size_t ListDyn_add(struct SEA_ListDyn* da, const void* ptr) {
    void* slot = ListDyn_alloc(da);
    if (slot && ptr) {
        memcpy(slot, ptr, da->elementSize);
    }
    return da->count;
}

static size_t ListDyn_insert(struct SEA_ListDyn* da, size_t index, const void* ptr) {
    if (index > da->count) index = da->count;
    
    ListDyn_ensureCapacity(da, da->count + 1);
    
    // Shift elements to make room
    if (index < da->count) {
        char* src = (char*)da->data + (index * da->elementSize);
        char* dst = src + da->elementSize;
        const size_t moveBytes = (da->count - index) * da->elementSize;
        memmove(dst, src, moveBytes);
    }
    
    da->count++;
    void* slot = ListDyn_get(da, index);
    if (slot && ptr) {
        memcpy(slot, ptr, da->elementSize);
    }
    return da->count;
}

static void ListDyn_remove(struct SEA_ListDyn* da, const size_t index) {
    if (index >= da->count) return;
    
    // Shift elements to fill the gap
    if (index < da->count - 1) {
        char* dst = (char*)da->data + (index * da->elementSize);
        const char* src = dst + da->elementSize;
        const size_t moveBytes = (da->count - index - 1) * da->elementSize;
        memmove(dst, src, moveBytes);
    }
    
    da->count--;
}

static size_t ListDyn_count(const struct SEA_ListDyn* da) {
    return da->count;
}

static size_t ListDyn_capacity(const struct SEA_ListDyn* da) {
    return da->capacity;
}

static void ListDyn_reserve(struct SEA_ListDyn* da, const size_t capacity) {
    ListDyn_ensureCapacity(da, capacity);
}

static void ListDyn_shrink(struct SEA_ListDyn* da) {
    if (da->count == 0) {
        if (da->data) {
            SEA_Allocator.free(da->allocator, da->data);
            da->data = NULL;
            da->capacity = 0;
        }
        return;
    }
    
    if (da->capacity > da->count) {
        void* newData = SEA_Allocator.alloc(da->allocator, da->count * da->elementSize);
        if (newData) {
            memcpy(newData, da->data, da->count * da->elementSize);
            SEA_Allocator.free(da->allocator, da->data);
            da->data = newData;
            da->capacity = da->count;
        }
    }
}

static void ListDyn_clear(struct SEA_ListDyn* da) {
    da->count = 0;
}

static void ListDyn_free(struct SEA_ListDyn* da) {
    if (da->data) {
        SEA_Allocator.free(da->allocator, da->data);
        da->data = NULL;
    }
    da->count = 0;
    da->capacity = 0;
}

const struct SEA_ListDyn_CLS SEA_ListDyn = {
    .get = ListDyn_get,
    .alloc = ListDyn_alloc,
    .add = ListDyn_add,
    .insert = ListDyn_insert,
    .remove = ListDyn_remove,
    .count = ListDyn_count,
    .capacity = ListDyn_capacity,
    .reserve = ListDyn_reserve,
    .shrink = ListDyn_shrink,
    .clear = ListDyn_clear,
    .free = ListDyn_free,
};
