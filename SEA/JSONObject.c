#include "JSONObject.h"

#include <string.h>

#include "Align.h"
#include "Allocator.h"
#include "Error.h"
#include "JSONValue.h"
#include "Time.h"


// ===================================
// MARK: Utilities
// ===================================

static size_t HashString(const char* s) {
	size_t h = 5381;
	while (*s) {
		h = ((h << 5) + h) + (unsigned char)*s++;
	}
	return h;
}

// ===================================
// MARK: JsonObject
// ===================================

static struct SEA_JSONValue* JSONObject_New(struct SEA_Allocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	uint8_t* buffer = SEA_Allocator.alloc(
		alloc,
		sizeof(struct SEA_JSONValue)
		+ sizeof(struct SEA_JSONObject)
		+ sizeof(SEA_JSONObjectEntry*) * SEA_JSON_OBJECT_BUCKETS_NUMBERS
	);
	if (!buffer) return NULL;

	struct SEA_JSONValue* value = (struct SEA_JSONValue*)buffer;
	value->object = (struct SEA_JSONObject*)(buffer + sizeof(struct SEA_JSONValue));
	value->object->buckets = (SEA_JSONObjectEntry**)(buffer + sizeof(struct SEA_JSONValue) + sizeof(struct SEA_JSONObject));
	memset(value->object->buckets, 0, SEA_JSON_OBJECT_BUCKETS_NUMBERS * sizeof(SEA_JSONObjectEntry*));

	value->type = SEA_JSON_OBJECT;
	value->object->bucketCount = SEA_JSON_OBJECT_BUCKETS_NUMBERS;
	value->object->size = 0;
	value->object->ref_count = 1;

	return value;
}

static enum SEA_ErrorType JSONObject_put(struct SEA_JSONObject* self, const char* key, struct SEA_JSONValue* value, struct SEA_Allocator* alloc) {
	if (!self) {
		SEA_Error.SetError(SEA_ERROR_GENERIC_SELF_NULL);
		return SEA_ERROR_GENERIC_SELF_NULL;
	}

	if (!key || !value || !alloc || !alloc->alloc) {
		SEA_Error.SetError(SEA_ERROR_GENERIC_ARGUMENT_NULL);
		return SEA_ERROR_GENERIC_ARGUMENT_NULL;
	}

	if (!alloc->alloc) {
		SEA_Error.SetError(SEA_ERROR_ALLOCATOR_ALLOC_FUNC_NULL);
		return SEA_ERROR_ALLOCATOR_ALLOC_FUNC_NULL;
	}

	const size_t idx = HashString(key) % self->bucketCount;
	SEA_JSONObjectEntry** bucket = &self->buckets[idx];

	// Check if the key already exists
	for (SEA_JSONObjectEntry* e = *bucket; e != NULL; e = e->next) {
		if (strcmp(e->key, key) == 0) {
			SEA_JSONValue.free(e->value, alloc);
			e->value = value;
			return SEA_ERROR_NONE;
		}
	}

	SEA_JSONObjectEntry* e = SEA_Allocator.alloc(alloc, sizeof(SEA_JSONObjectEntry));
	e->key_len = strlen(key);
	e->key = SEA_Allocator.strdup(alloc, key, e->key_len + 1);
	e->value = value;
	e->next = *bucket;
	*bucket = e;
	self->size++;

	return SEA_ERROR_NONE;
}

static struct SEA_JSONValue* JSONObject_get(const struct SEA_JSONObject* self, const char* key) {
	const size_t idx = HashString(key) % self->bucketCount;
	for (const SEA_JSONObjectEntry* e = self->buckets[idx]; e; e = e->next) {
		if (strcmp(e->key, key) == 0) {
			return e->value;
		}
	}
	return NULL;
}


static bool JSONObject_has(const struct SEA_JSONObject* self, const char* key) {
	if (!self || !key) return false;
	const size_t idx = HashString(key) % self->bucketCount;
	for (const SEA_JSONObjectEntry* e = self->buckets[idx]; e; e = e->next) {
		if (strcmp(e->key, key) == 0) return true;
	}
	return false;
}

static const char** JSONObject_keys(const struct SEA_JSONObject* self, const struct SEA_Allocator* alloc) {
	if (!self || !alloc) return NULL;
	const size_t count = self->size;
	const char** keys = SEA_Allocator.allocAligned(alloc, count * sizeof(char*), SEA_alignof(char *));
	if (!keys) return NULL;
	size_t idx = 0;
	for (size_t i = 0; i < self->bucketCount; i++) {
		for (const SEA_JSONObjectEntry* e = self->buckets[i]; e != NULL; e = e->next) {
			keys[idx++] = e->key;
		}
	}
	return keys;
}

static size_t JSONObject_size(const struct SEA_JSONObject* self) {
	if (!self) return 0;
	return self->size;
}

static bool JSONObject_remove(struct SEA_JSONObject* self, const char* key, struct SEA_Allocator* alloc) {
	if (!self || !key || !alloc) return false;

	const size_t idx = HashString(key) % self->bucketCount;
	SEA_JSONObjectEntry** prev = &self->buckets[idx];
	for (SEA_JSONObjectEntry* e = *prev; e; e = e->next) {
		if (strcmp(e->key, key) == 0) {
			*prev = e->next;
			SEA_Allocator.free(alloc, e->key);
			SEA_JSONValue.free(e->value, alloc);
			self->size--;
			return true;
		}
		prev = &e->next;
	}
	return false;
}

static void JSONObject_free(struct SEA_JSONValue* self, struct SEA_Allocator* alloc) {
	self->object->ref_count--;
	if (self->object->ref_count > 0) return;

	for (size_t i = 0; i < self->object->bucketCount; i++) {
		SEA_JSONObjectEntry* next = self->object->buckets[i];
		for (SEA_JSONObjectEntry* e = next; e != NULL; e = next) {
			next = e->next;
			SEA_JSONValue.free(e->value, alloc);
			SEA_Allocator.free(alloc, e->key);
			SEA_Allocator.free(alloc, e);
		}
	}

	SEA_Allocator.free(alloc, self);
}

static struct SEA_JSONValue* JSONObject_asJSONValue(const struct SEA_JSONObject* self) {
	if (!self) return NULL;
	return (struct SEA_JSONValue*)((char*)self - sizeof(struct SEA_JSONValue));
}

const struct SEA_JSONObject_CLS SEA_JSONObject = {
	.New = JSONObject_New,
	.put = JSONObject_put,
	.get = JSONObject_get,
	.has = JSONObject_has,
	.size = JSONObject_size,
	.keys = JSONObject_keys,
	.remove = JSONObject_remove,
	.free = JSONObject_free,
	.asJSONValue = JSONObject_asJSONValue,
};
