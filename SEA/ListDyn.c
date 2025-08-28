#include "ListDyn.h"

#include "Iterator.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static void SEA_ListDyn_ensureCapacity(SEA_ListDyn* self, const size_t minCapacity) {
    if (self->capacity >= minCapacity) return;
    
    size_t newCapacity = self->capacity == 0 ? SEA_DYNAMIC_ARRAY_INITIAL_CAPACITY : self->capacity;
    while (newCapacity < minCapacity) {
        newCapacity *= SEA_DYNAMIC_ARRAY_GROWTH_FACTOR;
    }
    
    void* newData = SEA_Allocator_alloc(self->allocator, newCapacity * self->elementSize);
    if (!newData) return;
    
    if (self->data) {
        memcpy(newData, self->data, self->count * self->elementSize);
        SEA_Allocator_free(self->allocator, self->data);
    }
    
    self->data = newData;
    self->capacity = newCapacity;
}

void* SEA_ListDyn_get(const SEA_ListDyn* self, const size_t index) {
    if (index >= self->count) return NULL;
    return (char*)self->data + (index * self->elementSize);
}

void* SEA_ListDyn_alloc(SEA_ListDyn* self) {
    SEA_ListDyn_ensureCapacity(self, self->count + 1);
    self->count++;
    return SEA_ListDyn_get(self, self->count - 1);
}

size_t SEA_ListDyn_add(SEA_ListDyn* self, const void* ptr) {
    void* slot = SEA_ListDyn_alloc(self);
    if (slot && ptr) {
        memcpy(slot, ptr, self->elementSize);
    }
    return self->count;
}

size_t SEA_ListDyn_insert(SEA_ListDyn* self, size_t index, const void* ptr) {
    if (index > self->count) index = self->count;
    
    SEA_ListDyn_ensureCapacity(self, self->count + 1);
    
    // Shift elements to make room
    if (index < self->count) {
        char* src = (char*)self->data + (index * self->elementSize);
        char* dst = src + self->elementSize;
        const size_t moveBytes = (self->count - index) * self->elementSize;
        memmove(dst, src, moveBytes);
    }
    
    self->count++;
    void* slot = SEA_ListDyn_get(self, index);
    if (slot && ptr) {
        memcpy(slot, ptr, self->elementSize);
    }
    return self->count;
}

void SEA_ListDyn_remove(SEA_ListDyn* self, const size_t index) {
    if (index >= self->count) return;
    
    // Shift elements to fill the gap
    if (index < self->count - 1) {
        char* dst = (char*)self->data + (index * self->elementSize);
        const char* src = dst + self->elementSize;
        const size_t moveBytes = (self->count - index - 1) * self->elementSize;
        memmove(dst, src, moveBytes);
    }
    
    self->count--;
}

size_t SEA_ListDyn_count(const SEA_ListDyn* self) {
    return self->count;
}

size_t SEA_ListDyn_capacity(const SEA_ListDyn* self) {
    return self->capacity;
}

void SEA_ListDyn_reserve(SEA_ListDyn* self, const size_t capacity) {
    SEA_ListDyn_ensureCapacity(self, capacity);
}

void SEA_ListDyn_shrink(SEA_ListDyn* self) {
    if (self->count == 0) {
        if (self->data) {
            SEA_Allocator_free(self->allocator, self->data);
            self->data = NULL;
            self->capacity = 0;
        }
        return;
    }
    
    if (self->capacity > self->count) {
        void* newData = SEA_Allocator_alloc(self->allocator, self->count * self->elementSize);
        if (newData) {
            memcpy(newData, self->data, self->count * self->elementSize);
            SEA_Allocator_free(self->allocator, self->data);
            self->data = newData;
            self->capacity = self->count;
        }
    }
}

void SEA_ListDyn_clear(SEA_ListDyn* self) {
    self->count = 0;
}

void SEA_ListDyn_free(SEA_ListDyn* self) {
    if (self->data) {
        SEA_Allocator_free(self->allocator, self->data);
        self->data = NULL;
    }
    self->count = 0;
    self->capacity = 0;
}

// =====================================
// MARK: Iterator
// =====================================

struct IteratorData {
    SEA_ListDyn* list;
    size_t currentIndex;
    SEA_Allocator allocator;
};

static void* Iterator_next(const SEA_Iterator* self) {
    struct IteratorData* data = self->data;
    if (data->currentIndex >= data->list->count) {
        return NULL;
    }
    void* element = SEA_ListDyn_get(data->list, data->currentIndex);
    data->currentIndex++;
    return element;
}

static bool Iterator_hasNext(const SEA_Iterator* self) {
    const struct IteratorData* data = self->data;
    return data->currentIndex < data->list->count;
}

static void Iterator_destroy(SEA_Iterator* self) {
    const struct IteratorData* data = self->data;
    if (data != NULL) {
        SEA_Allocator allocator = data->allocator;
        self->data = NULL;
        SEA_Allocator_free(&allocator, self);
    }
}

SEA_Iterator* SEA_ListDyn_iterator(SEA_ListDyn* self, SEA_Allocator *allocatorOverride) {
    SEA_Allocator* allocator = allocatorOverride == NULL ? self->allocator : allocatorOverride;

    const size_t totalAlloc = sizeof(struct SEA_Iterator) + sizeof(struct IteratorData);
    SEA_Iterator* iter = SEA_Allocator_alloc(allocator, totalAlloc);

    struct IteratorData* data = (struct IteratorData*)(
        (uint8_t*)iter + sizeof(struct SEA_Iterator)
    );

    if (data) {
        iter->next = Iterator_next;
        iter->hasNext = Iterator_hasNext;
        iter->destroy = Iterator_destroy;
        iter->data = data;

        data->allocator = *allocator;
        data->list = self;
        data->currentIndex = 0;
    }

    return iter;
}