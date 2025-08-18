#ifndef SEALIB_ITERATOR_H
#define SEALIB_ITERATOR_H
#include <stdbool.h>

struct SEA_Iterator {
	void* data;
	void* (*next)(const struct SEA_Iterator* iter);
	bool (*hasNext)(const struct SEA_Iterator* iter);
	void (*destroy)(struct SEA_Iterator* iter);
};

// Macro for foreach loop
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

#endif //SEALIB_ITERATOR_H