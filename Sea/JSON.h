#ifndef SEALIB_JSON_H
#define SEALIB_JSON_H

#include "Allocator.h"

#include <stdbool.h>

typedef enum {
	SEAJSON_NULL,
	SEAJSON_BOOL,
	SEAJSON_NUMBER,
	SEAJSON_STRING,
	SEAJSON_ARRAY,
	SEAJSON_OBJECT
} SeaJsonType;

struct SeaJsonArray {
	struct SeaJsonValue** items;
	size_t count;
	size_t capacity;
	size_t ref_count;
};

struct SeaJsonObject {
	char** keys;
	struct SeaJsonValue** values;
	size_t count;
	size_t capacity;
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
	struct SeaJsonValue* (*parseString)(const char* string, struct SeaAllocator* allocator);

	struct SeaJsonValue* (*getNull)();
	struct SeaJsonValue* (*createBool)(bool value, struct SeaAllocator* allocator);
	struct SeaJsonValue* (*createNumber)(double value, struct SeaAllocator* allocator);
	struct SeaJsonValue* (*createString)(const char* value, struct SeaAllocator* allocator);
	struct SeaJsonValue* (*createArray)(struct SeaAllocator* allocator);
	struct SeaJsonValue* (*createObject)(struct SeaAllocator* allocator);

	char* (*toString)(const struct SeaJsonValue* self, struct SeaAllocator* allocator);
	void (*free)(struct SeaJsonValue* self, struct SeaAllocator* allocator);
} SeaJsonValue;

extern const struct SeaJsonObject_CLS {
	void (*put)(struct SeaJsonObject* self, const char* key, struct SeaJsonValue* value, struct SeaAllocator* alloc);
	bool (*has)(const struct SeaJsonObject* self, const char* key);
	size_t (*size)(const struct SeaJsonObject* self);

	/** Increase refcount if it's Object or Array */
	struct SeaJsonValue* (*get)(const struct SeaJsonObject* self, const char* key);
	/** Decrease refcount of value if it's Object or Array */
	bool (*remove)(struct SeaJsonObject* self, const char* key, struct SeaAllocator* allocator);
	/** Decrease refcount of self */
	void (*free)(struct SeaJsonObject* self, struct SeaAllocator* allocator);
} SeaJsonObject;

extern const struct SeaJsonArray_CLS {
	void (*add)(struct SeaJsonArray* self, struct SeaJsonValue* value, struct SeaAllocator* allocator);
	/** Increase refcount if it's Object or Array */
	struct SeaJsonValue* (*get)(const struct SeaJsonArray* self, size_t index);
} SeaJsonArray;

#endif//SEALIB_JSON_H