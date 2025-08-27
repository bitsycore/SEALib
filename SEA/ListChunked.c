#include "ListChunked.h"

#include <string.h>

static struct SEA_ListChunk* ListChunked_allocChunk(const struct SEA_ListChunked* ca) {
    struct SEA_ListChunk* chunk = SEA_Allocator_alloc(ca->allocator, sizeof(struct SEA_ListChunk));
    if (!chunk) return NULL;
    
    chunk->data = SEA_Allocator_alloc(ca->allocator, ca->chunkCapacity * ca->elementSize);
    if (!chunk->data) {
        SEA_Allocator_free(ca->allocator, chunk);
        return NULL;
    }
    
    chunk->count = 0;
    chunk->next = NULL;
    return chunk;
}

static void* ListChunked_get(const struct SEA_ListChunked* ca, const size_t index) {
    if (index >= ca->totalCount) return NULL;

    const struct SEA_ListChunk* chunk = ca->head;
    size_t currentIndex = 0;
    
    while (chunk) {
        if (index < currentIndex + chunk->count) {
            const size_t chunkIndex = index - currentIndex;
            return (char*)chunk->data + (chunkIndex * ca->elementSize);
        }
        currentIndex += chunk->count;
        chunk = chunk->next;
    }
    
    return NULL;
}

static void* ListChunked_alloc(struct SEA_ListChunked* ca) {
    // If no chunks exist or last chunk is full, allocate new chunk
    if (!ca->tail || ca->tail->count >= ca->chunkCapacity) {
        struct SEA_ListChunk* newChunk = ListChunked_allocChunk(ca);
        if (!newChunk) return NULL;
        
        if (!ca->head) {
            ca->head = ca->tail = newChunk;
        } else {
            ca->tail->next = newChunk;
            ca->tail = newChunk;
        }
    }
    
    void* slot = (char*)ca->tail->data + (ca->tail->count * ca->elementSize);
    ca->tail->count++;
    ca->totalCount++;
    return slot;
}

static size_t ListChunked_add(struct SEA_ListChunked* ca, const void* ptr) {
    void* slot = ListChunked_alloc(ca);
    if (slot && ptr) {
        memcpy(slot, ptr, ca->elementSize);
    }
    return ca->totalCount;
}

static size_t ListChunked_count(const struct SEA_ListChunked* ca) {
    return ca->totalCount;
}

static void ListChunked_clear(struct SEA_ListChunked* ca) {
    struct SEA_ListChunk* chunk = ca->head;
    while (chunk) {
        chunk->count = 0;
        chunk = chunk->next;
    }
    ca->totalCount = 0;
}

static void ListChunked_free(struct SEA_ListChunked* ca) {
    struct SEA_ListChunk* chunk = ca->head;
    while (chunk) {
        struct SEA_ListChunk* next = chunk->next;
        SEA_Allocator_free(ca->allocator, chunk->data);
        SEA_Allocator_free(ca->allocator, chunk);
        chunk = next;
    }
    ca->head = ca->tail = NULL;
    ca->totalCount = 0;
}

const struct SEA_ListChunked_CLS SEA_ListChunked = {
    .get = ListChunked_get,
    .alloc = ListChunked_alloc,
    .add = ListChunked_add,
    .count = ListChunked_count,
    .clear = ListChunked_clear,
    .free = ListChunked_free,
};
