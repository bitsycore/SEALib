#ifndef SEA_JSONARRAY_H
#define SEA_JSONARRAY_H

#include "../Allocator.h"

#include <stddef.h>

#define SEA_JSON_ARRAY_INITIAL_CAPACITY 8

struct SEA_JSONArray {
	struct SEA_JSONValue** items;
	size_t count;
	size_t capacity;
	size_t ref_count;
};

extern const struct SEA_JSONArray_CLS {
	// STATIC
	/**Refcount(ret +)*/
	struct SEA_JSONValue* (*New)(struct SEA_Allocator* alloc);

	// INSTANCE
	void (*add)(struct SEA_JSONArray* self, struct SEA_JSONValue* value, struct SEA_Allocator* allocator);
	/**Refcount(ret +)*/
	struct SEA_JSONValue* (*get)(const struct SEA_JSONArray* self, size_t index);
	void (*free)(struct SEA_JSONValue * self, struct SEA_Allocator * alloc);
} SEA_JSONArray;

#endif//SEA_JSONARRAY_H
