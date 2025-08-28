#ifndef SEA_JSON_ARRAY_H
#define SEA_JSON_ARRAY_H

#include "Config/CompConfig.h"

#include "Allocator.h"
#include "JSONValue.h"

#include <stddef.h>

#define SEA_JSON_ARRAY_INITIAL_CAPACITY 8

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

// =======================================
// MARK: Alias
// =======================================

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

// =========================
// Types
typedef SEA_JSONArray JSONArray;

// =========================
// Static
static inline JSONValue* JSONArray_New(Allocator* alloc) { return SEA_JSONArray_New(alloc); }

// =========================
// Instance
static inline void JSONArray_add(JSONArray* self, JSONValue* value, Allocator* allocator) { SEA_JSONArray_add(self, value, allocator); }
static inline JSONValue* JSONArray_get(const JSONArray* self, const size_t index) { return SEA_JSONArray_get(self, index); }
static inline void JSONArray_free(JSONValue* self, Allocator* alloc) { SEA_JSONArray_free(self, alloc); }
static inline JSONValue* JSONArray_asJSONValue(const JSONArray* self) { return SEA_JSONArray_asJSONValue(self); }

#endif

#endif//SEA_JSON_ARRAY_H