#ifndef SEA_JSON_OBJECT_H
#define SEA_JSON_OBJECT_H

#include "Allocator.h"
#include "Error.h"
#include "JSONValue.h"
#include "Config/CompConfig.h"

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

// =======================================
// MARK: Alias
// =======================================

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

// =========================
// Types
typedef SEA_JSONObject JSONObject;

// =========================
// Static
static inline JSONValue* JSONObject_New(Allocator* alloc) { return SEA_JSONObject_New(alloc); }

// =========================
// Instance
static inline JSONValue* JSONObject_get(const JSONObject* self, const char* key) { return SEA_JSONObject_get(self, key); }

static inline enum SEA_ErrorType JSONObject_put(JSONObject* self, const char* key, JSONValue* value, Allocator* allocator) {
	return SEA_JSONObject_put(self, key, value, allocator);
}

static inline JSONValue* JSONObject_getOrPut(JSONObject* self, const char* key, JSONValue* value, Allocator* allocator) {
	return SEA_JSONObject_getOrPut(self, key, value, allocator);
}

static inline JSONValue* JSONObject_newRef(const JSONObject* self) { return SEA_JSONObject_newRef(self); }
static inline JSONValue* JSONObject_clone(const JSONObject* self, Allocator* allocator) { return SEA_JSONObject_clone(self, allocator); }
static inline bool JSONObject_has(const JSONObject* self, const char* key) { return SEA_JSONObject_has(self, key); }
static inline const char** JSONObject_keys(const JSONObject* self, Allocator* allocator) { return SEA_JSONObject_keys(self, allocator); }
static inline size_t JSONObject_size(const JSONObject* self) { return SEA_JSONObject_size(self); }
static inline bool JSONObject_remove(JSONObject* self, const char* key, Allocator* allocator) { return SEA_JSONObject_remove(self, key, allocator); }
static inline void JSONObject_free(JSONValue* self, Allocator* alloc) { SEA_JSONObject_free(self, alloc); }
static inline JSONValue* JSONObject_asJSONValue(const JSONObject* self) { return SEA_JSONObject_asJSONValue(self); }

#endif

#endif //SEA_JSON_OBJECT_H
