#ifndef SEA_JSON_OBJECT_H
#define SEA_JSON_OBJECT_H

#include "Config/CompConfig.h"
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
	struct SEA_JSONValue* value;
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

struct SEA_JSONValue* SEA_JSONObject_New(struct SEA_Allocator* alloc);

// =======================================
// MARK: Instance
// =======================================

struct SEA_JSONValue* SEA_JSONObject_get(const struct SEA_JSONObject* self, const char* key);
enum SEA_ErrorType SEA_JSONObject_put(struct SEA_JSONObject* self, const char* key, struct SEA_JSONValue* value, struct SEA_Allocator* allocator);
struct SEA_JSONValue* SEA_JSONObject_getOrPut(struct SEA_JSONObject* self, const char* key, struct SEA_JSONValue* value, struct SEA_Allocator* allocator);
struct SEA_JSONValue* SEA_JSONObject_newRef(const struct SEA_JSONObject* self);
struct SEA_JSONValue* SEA_JSONObject_clone(const struct SEA_JSONObject* self, struct SEA_Allocator* allocator);
bool SEA_JSONObject_has(const struct SEA_JSONObject* self, const char* key);
const char** SEA_JSONObject_keys(const struct SEA_JSONObject* self, struct SEA_Allocator* allocator);
size_t SEA_JSONObject_size(const struct SEA_JSONObject* self);
bool SEA_JSONObject_remove(struct SEA_JSONObject* self, const char* key, struct SEA_Allocator* allocator);
void SEA_JSONObject_free(struct SEA_JSONValue * self, struct SEA_Allocator * alloc);
struct SEA_JSONValue* SEA_JSONObject_asJSONValue(const struct SEA_JSONObject* self);

// =======================================
// MARK: Alias
// =======================================

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

typedef SEA_JSONObject JSONObject;

// Static functions
static inline JSONValue* JSONObject_New(Allocator* alloc) {
	return SEA_JSONObject_New(alloc);
}

// Instance functions
static inline JSONValue* JSONObject_get(const JSONObject* self, const char* key) {
	return SEA_JSONObject_get(self, key);
}

static inline enum SEA_ErrorType JSONObject_put(JSONObject* self, const char* key, JSONValue* value, Allocator* allocator) {
	return SEA_JSONObject_put(self, key, value, allocator);
}

static inline JSONValue* JSONObject_getOrPut(JSONObject* self, const char* key, JSONValue* value, Allocator* allocator) {
	return SEA_JSONObject_getOrPut(self, key, value, allocator);
}

static inline JSONValue* JSONObject_newRef(const JSONObject* self) {
	return SEA_JSONObject_newRef(self);
}

static inline JSONValue* JSONObject_clone(const JSONObject* self, Allocator* allocator) {
	return SEA_JSONObject_clone(self, allocator);
}

static inline bool JSONObject_has(const JSONObject* self, const char* key) {
	return SEA_JSONObject_has(self, key);
}

static inline const char** JSONObject_keys(const JSONObject* self, Allocator* allocator) {
	return SEA_JSONObject_keys(self, allocator);
}

static inline size_t JSONObject_size(const JSONObject* self) {
	return SEA_JSONObject_size(self);
}

static inline bool JSONObject_remove(JSONObject* self, const char* key, Allocator* allocator) {
	return SEA_JSONObject_remove(self, key, allocator);
}

static inline void JSONObject_free(JSONValue* self, Allocator* alloc) {
	SEA_JSONObject_free(self, alloc);
}

static inline JSONValue* JSONObject_asJSONValue(const JSONObject* self) {
	return SEA_JSONObject_asJSONValue(self);
}

#endif

#endif //SEA_JSON_OBJECT_H
