#ifndef SEALIB_ITERATOR_H
#define SEALIB_ITERATOR_H

#include "Config/CompConfig.h"

#include <stdbool.h>

typedef struct SEA_Iterator {
	void* data;
	void* (*next)(const struct SEA_Iterator* iter);
	bool (*hasNext)(const struct SEA_Iterator* iter);
	void (*destroy)(struct SEA_Iterator* iter);
} SEA_Iterator;

// =======================================
// MARK: Macro
// =======================================

#define SEA_Iterator_foreach(item_type, _iterator) \
    for(\
        struct {\
            item_type* value;\
            struct SEA_Iterator* iterator;\
            bool should_destroy;\
        } it = { \
            .iterator = (_iterator), \
            .value = NULL, \
            .should_destroy = false \
        };\
        !it.should_destroy && \
        (it.value = (item_type*)(it.iterator)->next(it.iterator)) != NULL; \
        it.should_destroy = !it.iterator->hasNext(it.iterator), \
        it.should_destroy ? (it.iterator->destroy(it.iterator), it.value) : it.value \
    )

// =======================================
// MARK: Alias
// =======================================

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

// =========================
// Types
typedef SEA_Iterator Iterator;

// =========================
// Instance
#define Iterator_foreach SEA_Iterator_foreach

#endif

#endif //SEALIB_ITERATOR_H