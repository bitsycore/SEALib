#ifndef SEA_JSON_VALUE_H
#define SEA_JSON_VALUE_H

#include "Allocator.h"
#include "JSONType.h"

#include <stdbool.h>

// =======================================
// MARK: Types
// =======================================

typedef struct SEA_JSONValue {
	union {
		bool boolean;
		double number;
		char* string;
		struct SEA_JSONArray* array;
		struct SEA_JSONObject* object;
	};

	SEA_JSONType type;
} SEA_JSONValue;

// =======================================
// MARK: Static
// =======================================

SEA_JSONValue* SEA_JSONValue_FromString(const char* string, size_t len, SEA_Allocator* allocator);
SEA_JSONValue* SEA_JSONValue_CreateNull();
SEA_JSONValue* SEA_JSONValue_CreateBool(bool value);
SEA_JSONValue* SEA_JSONValue_CreateNumber(double value, SEA_Allocator* allocator);
SEA_JSONValue* SEA_JSONValue_CreateString(const char* value, SEA_Allocator* allocator);
SEA_JSONValue* SEA_JSONValue_CreateArray(SEA_Allocator* allocator);
SEA_JSONValue* SEA_JSONValue_CreateObject(SEA_Allocator* allocator);

// =======================================
// MARK: Instance
// =======================================

char* SEA_JSONValue_toString(const SEA_JSONValue* self, SEA_Allocator* allocator);
void SEA_JSONValue_free(SEA_JSONValue* self, SEA_Allocator* allocator);

#endif//SEA_JSON_VALUE_H
