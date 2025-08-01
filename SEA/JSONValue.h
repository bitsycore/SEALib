#ifndef SEA_JSON_VALUE_H
#define SEA_JSON_VALUE_H

#include "JSONType.h"
#include "Allocator.h"

#include <stdbool.h>
#include <stdint.h>

struct SEA_JSONValue {
	union {
		bool boolean;
		double number;
		char* string;
		struct SEA_JSONArray* array;
		struct SEA_JSONObject* object;
	};
	SEA_JSONType type;
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

#endif//SEA_JSON_VALUE_H