#ifndef SEALIB_ITERATOR_H
#define SEALIB_ITERATOR_H

#include <stdbool.h>

// =======================================
// MARK: Types
// =======================================

typedef struct SEA_Iterator {
	void* data;
	void* (*next)(const struct SEA_Iterator* self);
	bool (*hasNext)(const struct SEA_Iterator* self);
	void (*destroy)(struct SEA_Iterator* self);
} SEA_Iterator;

// =======================================
// MARK: Macro
// =======================================

#define SEA_Iterator_foreach(_item_type_, _iterator_) \
    for(\
        struct {\
            typeof(_item_type_)* value;\
            struct SEA_Iterator* iterator;\
            bool should_destroy;\
        } it = { \
            .iterator = (_iterator_), \
            .value = NULL, \
            .should_destroy = false \
        };\
        !it.should_destroy && (it.value = (typeof(it.value))(it.iterator)->next(it.iterator)) != NULL; \
        it.should_destroy = !it.iterator->hasNext(it.iterator), (it.should_destroy ? (it.iterator->destroy(it.iterator), NULL) : NULL) \
    )

#endif //SEALIB_ITERATOR_H