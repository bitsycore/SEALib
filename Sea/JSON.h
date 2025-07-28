#ifndef SEALIB_JSON_H
#define SEALIB_JSON_H

#include <stdbool.h>
#include <stdlib.h>

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
};

struct SeaJsonObject {
	char** keys;
	struct SeaJsonValue** values;
	size_t count;
	size_t capacity;
};

struct SeaJsonValue {
	SeaJsonType type;

	union {
		bool boolean;
		double number;
		char* string;
		struct SeaJsonArray array;
		struct SeaJsonObject object;
	};
};

extern const struct SeaJsonValue_CLS {
	struct SeaJsonValue* (*parse)(const char* json, struct SeaAllocator* allocator);
	char* (*toString)(struct SeaJsonValue* self, struct SeaAllocator* allocator);
	void (*free)(struct SeaJsonValue* self, struct SeaAllocator* alloc);
} SeaJsonValue;

extern const struct SeaJsonObject_CLS {
	void (*put)(struct SeaJsonValue* object, const char* key, struct SeaJsonValue* value, struct SeaAllocator* alloc);
	struct SeaJsonValue* (*get)(const struct SeaJsonValue* object, const char* key);
} SeaJsonObject;

extern const struct SeaJsonArray_CLS {
	void (*add)(struct SeaJsonValue* self, struct SeaJsonValue* value, struct SeaAllocator* alloc);
	struct SeaJsonValue* (*get)(const struct SeaJsonValue* self, size_t index);
} SeaJsonArray;

#endif//SEALIB_JSON_H
