#ifndef SEA_JSON_OBJECT_H
#define SEA_JSON_OBJECT_H

#include "Allocator.h"
#include "Error.h"
#include "JSONValue.h"

#include <stdbool.h>
#include <stddef.h>

// =======================================
// MARK: Config
// =======================================

#define SEA_JSON_OBJECT_BUCKETS_NUMBERS 31

// =======================================
// MARK: Types
// =======================================

typedef struct SEA_JSONObjectEntry {
	char* key;
	size_t key_len;
	size_t key_hash;
	SEA_JSONValue* value;
	struct SEA_JSONObjectEntry* next;
} SEA_JSONObjectEntry;

typedef struct SEA_JSONObject {
	SEA_JSONObjectEntry** buckets;
	size_t size;
	size_t bucketCount;
	size_t ref_count;
} SEA_JSONObject;

// =======================================
// MARK: Static
// =======================================

SEA_JSONValue* SEA_JSONObject_New(SEA_Allocator* alloc);

// =======================================
// MARK: Instance
// =======================================

SEA_JSONValue* SEA_JSONObject_get(const SEA_JSONObject* self, const char* key);
enum SEA_ErrorType SEA_JSONObject_put(SEA_JSONObject* self, const char* key, SEA_JSONValue* value, SEA_Allocator* allocator);
SEA_JSONValue* SEA_JSONObject_getOrPut(SEA_JSONObject* self, const char* key, SEA_JSONValue* value, SEA_Allocator* allocator);
SEA_JSONValue* SEA_JSONObject_newRef(const SEA_JSONObject* self);
SEA_JSONValue* SEA_JSONObject_clone(const SEA_JSONObject* self, SEA_Allocator* allocator);
bool SEA_JSONObject_has(const SEA_JSONObject* self, const char* key);
const char** SEA_JSONObject_keys(const SEA_JSONObject* self, SEA_Allocator* allocator);
size_t SEA_JSONObject_size(const SEA_JSONObject* self);
bool SEA_JSONObject_remove(SEA_JSONObject* self, const char* key, SEA_Allocator* allocator);
void SEA_JSONObject_free(SEA_JSONValue* self, SEA_Allocator* alloc);
SEA_JSONValue* SEA_JSONObject_asJSONValue(const SEA_JSONObject* self);

#endif //SEA_JSON_OBJECT_H
