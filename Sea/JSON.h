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
	SeaJsonType type;

	union {
		bool boolean;
		double number;
		char* string;
		struct SeaJsonArray* array;
		struct SeaJsonObject* object;
	};
};

extern const struct SeaJsonValue_CLS {
	struct SeaJsonValue* (*parseString)(const char* string, struct SeaAllocator* allocator);
	struct SeaJsonValue* (*createNull)(struct SeaAllocator* alloc);
	struct SeaJsonValue* (*createBool)(bool val, struct SeaAllocator* alloc);
	struct SeaJsonValue* (*createNumber)(double val, struct SeaAllocator* alloc);
	struct SeaJsonValue* (*createString)(const char* val, struct SeaAllocator* alloc);
	struct SeaJsonValue* (*createArray)(struct SeaAllocator* alloc);
	struct SeaJsonValue* (*createObject)(struct SeaAllocator* alloc);

	char* (*toString)(const struct SeaJsonValue* self, struct SeaAllocator* alloc);
	void (*free)(struct SeaJsonValue* self, struct SeaAllocator* alloc);
} SeaJsonValue;

extern const struct SeaJsonObject_CLS {
	void (*put)(struct SeaJsonValue* self, const char* key, struct SeaJsonValue* value, struct SeaAllocator* alloc);
	/** Increase refcount if it's Object or Array */
	struct SeaJsonValue* (*get)(const struct SeaJsonValue* self, const char* key);
	bool (*has)(const struct SeaJsonValue* self, const char* key);
	size_t (*size)(const struct SeaJsonValue* self);
	/** Decrease refcount if it's Object or Array */
	bool (*remove)(struct SeaJsonValue* self, const char* key, struct SeaAllocator* alloc);
} SeaJsonObject;

extern const struct SeaJsonArray_CLS {
	void (*add)(struct SeaJsonValue* self, struct SeaJsonValue* value, struct SeaAllocator* alloc);
	/** Increase refcount if it's Object or Array */
	struct SeaJsonValue* (*get)(const struct SeaJsonValue* self, size_t index);
} SeaJsonArray;

#endif//SEALIB_JSON_H