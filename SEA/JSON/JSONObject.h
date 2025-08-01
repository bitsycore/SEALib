#ifndef SEA_JSONOBJECT_H
#define SEA_JSONOBJECT_H

#include "../Allocator.h"

#include <stdbool.h>
#include <stddef.h>

#define SEA_JSON_OBJECT_BUCKETS_NUMBERS 31

typedef struct SEA_JSONObjectEntry {
	char*                       key;
	size_t                      key_len;
	size_t                      key_hash;
	struct SEA_JSONValue*        value;
	struct SEA_JSONObjectEntry*  next;
} SEA_JSONObjectEntry;

struct SEA_JSONObject {
	SEA_JSONObjectEntry** buckets;
	size_t size;
	size_t bucketCount;
	size_t ref_count;
};

extern const struct SEA_JSONObject_CLS {
	// =====================================
	// STATIC
	// =====================================
	/**Refcount(ret +)*/
	struct SEA_JSONValue* (*New)(struct SEA_Allocator* alloc);

	// =====================================
	// INSTANCE
	// =====================================
	/**Refcount(ret +)*/
	struct SEA_JSONValue* (*get)(const struct SEA_JSONObject* self, const char* key);
	enum SEA_ErrorType (*put)(struct SEA_JSONObject* self, const char* key, struct SEA_JSONValue* value, struct SEA_Allocator* allocator);
	/**Refcount(ret?+)*/
	struct SEA_JSONValue* (*getOrPut)(struct SEA_JSONObject* self, const char* key, struct SEA_JSONValue* value, struct SEA_Allocator* allocator);
	/**Refcount(self+)*/
	struct SEA_JSONValue* (*newRef)(const struct SEA_JSONObject* self);
	/**Refcount(ret +)*/
	struct SEA_JSONValue* (*clone)(const struct SEA_JSONObject* self, struct SEA_Allocator* allocator);
	bool (*has)(const struct SEA_JSONObject* self, const char* key);
	const char** (*keys)(const struct SEA_JSONObject* self, const struct SEA_Allocator* allocator);
	size_t (*size)(const struct SEA_JSONObject* self);
	/**Refcount(ret -)*/
	bool (*remove)(struct SEA_JSONObject* self, const char* key, struct SEA_Allocator* allocator);
	/**Refcount(self-)*/
	void (*free)(const struct SEA_JSONValue * self, struct SEA_Allocator * alloc);
} SEA_JSONObject;

#endif //SEA_JSONOBJECT_H
