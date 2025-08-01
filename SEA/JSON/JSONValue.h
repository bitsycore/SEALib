#ifndef SEA_LIB_JSON_H
#define SEA_LIB_JSON_H

#include "../Allocator.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum {
	SEA_JSON_NULL,
	SEA_JSON_BOOL,
	SEA_JSON_NUMBER,
	SEA_JSON_STRING,
	SEA_JSON_ARRAY,
	SEA_JSON_OBJECT
} SEA_JsonType;

struct SEA_JSONValue {
	union {
		bool boolean;
		double number;
		char* string;
		struct SEA_JSONArray* array;
		struct SEA_JSONObject* object;
	};
	SEA_JsonType type;
	uint64_t timestamp;
};

extern const struct SEA_JSONValue_CLS {
	struct SEA_JSONValue* (*FromString)(const char* string, size_t len, struct SEA_Allocator* allocator);

	struct SEA_JSONValue* (*CreateNull)();
	struct SEA_JSONValue* (*CreateBool)(bool value);
	struct SEA_JSONValue* (*CreateNumber)(double value, struct SEA_Allocator* allocator);
	struct SEA_JSONValue* (*CreateString)(const char* value, struct SEA_Allocator* allocator);
	struct SEA_JSONValue* (*CreateArray)(struct SEA_Allocator* allocator);
	struct SEA_JSONValue* (*CreateObject)(struct SEA_Allocator* allocator);

	char* (*toString)(const struct SEA_JSONValue* self, struct SEA_Allocator* allocator);
	void (*free)(struct SEA_JSONValue* self, struct SEA_Allocator* allocator);
} SEA_JSONValue;

#endif//SEA_LIB_JSON_H