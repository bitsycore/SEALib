#ifndef SEA_LIB_JSON_H
#define SEA_LIB_JSON_H

#include "Allocator.h"

#include <stdbool.h>

typedef enum {
	SEA_JSON_NULL,
	SEA_JSON_BOOL,
	SEA_JSON_NUMBER,
	SEA_JSON_STRING,
	SEA_JSON_ARRAY,
	SEA_JSON_OBJECT
} SeaJsonType;

#define SEA_JSON_ARRAY_INITIAL_CAPACITY 8
#define SEA_JSON_OBJECT_BUCKETS_NUMBERS 31

struct SeaJsonArray {
	struct SeaJsonValue** items;
	size_t count;
	size_t capacity;
	size_t ref_count;
};

struct SeaJsonObject {
	void** buckets;
	size_t size;
	size_t bucketCount;
	size_t ref_count;
};

struct SeaJsonValue {
	union {
		bool boolean;
		double number;
		char* string;
		struct SeaJsonArray* array;
		struct SeaJsonObject* object;
	};
	SeaJsonType type;
};

extern const struct SeaJsonValue_CLS {
	struct SeaJsonValue* (*ParseJson)(const char* string, size_t len, struct SeaAllocator* allocator);

	struct SeaJsonValue* (*CreateNull)();
	struct SeaJsonValue* (*CreateBool)(bool value, struct SeaAllocator* allocator);
	struct SeaJsonValue* (*CreateNumber)(double value, struct SeaAllocator* allocator);
	struct SeaJsonValue* (*CreateString)(const char* value, struct SeaAllocator* allocator);
	struct SeaJsonValue* (*CreateArray)(struct SeaAllocator* allocator);
	struct SeaJsonValue* (*CreateObject)(struct SeaAllocator* allocator);

	char* (*toString)(const struct SeaJsonValue* self, struct SeaAllocator* allocator);
	void (*free)(struct SeaJsonValue* self, struct SeaAllocator* allocator);
} SeaJsonValue;

extern const struct SeaJsonObject_CLS {
	/** Increase refcount if it's Object or Array */
	struct SeaJsonValue* (*get)(const struct SeaJsonObject* self, const char* key);
	enum SeaErrorType (*put)(struct SeaJsonObject* self, const char* key, struct SeaJsonValue* value, struct SeaAllocator* alloc);
	bool (*has)(const struct SeaJsonObject* self, const char* key);
	size_t (*size)(const struct SeaJsonObject* self);
	/** Decrease refcount of value if it's Object or Array */
	bool (*remove)(struct SeaJsonObject* self, const char* key, struct SeaAllocator* allocator);
	/** Decrease refcount of self */
	void (*free)(struct SeaJsonObject* self, struct SeaAllocator* allocator);
} SeaJsonObject;

extern const struct SeaJsonArray_CLS {
	/** Increase refcount if it's Object or Array */
	struct SeaJsonValue* (*get)(const struct SeaJsonArray* self, size_t index);
	void (*add)(struct SeaJsonArray* self, struct SeaJsonValue* value, struct SeaAllocator* allocator);
} SeaJsonArray;

#endif//SEA_LIB_JSON_H