#ifndef SEA_JSON_ARRAY_H
#define SEA_JSON_ARRAY_H

#include "Allocator.h"
#include "JSONValue.h"

#include <stddef.h>

// =======================================
// MARK: Config
// =======================================

#define SEA_JSON_ARRAY_INITIAL_CAPACITY 8

// =======================================
// MARK: Types
// =======================================

typedef struct SEA_JSONArray {
	SEA_JSONValue** items;
	size_t count;
	size_t capacity;
	size_t ref_count;
} SEA_JSONArray;

// =======================================
// MARK: Static
// =======================================

SEA_JSONValue*  SEA_JSONArray_New(SEA_Allocator* alloc);

// =======================================
// MARK: Instance
// =======================================

void  SEA_JSONArray_add(SEA_JSONArray* self, SEA_JSONValue* value, SEA_Allocator* allocator);
SEA_JSONValue*  SEA_JSONArray_get(const SEA_JSONArray* self, size_t index);
void  SEA_JSONArray_free(SEA_JSONValue* self, SEA_Allocator* alloc);
SEA_JSONValue*  SEA_JSONArray_asJSONValue(const SEA_JSONArray* self);

#endif//SEA_JSON_ARRAY_H