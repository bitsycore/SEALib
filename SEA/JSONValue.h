#ifndef SEA_JSON_VALUE_H
#define SEA_JSON_VALUE_H

#include "Config/CompConfig.h"

#include "Allocator.h"
#include "JSONType.h"

#include <stdbool.h>

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

// =======================================
// MARK: Alias
// =======================================

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

// =========================
// Types
typedef SEA_JSONValue JSONValue;

// =========================
// Static
static inline JSONValue* JSONValue_FromString(const char* string, const size_t len, Allocator* allocator) { return SEA_JSONValue_FromString(string, len, allocator); }
static inline JSONValue* JSONValue_CreateNull() { return SEA_JSONValue_CreateNull(); }
static inline JSONValue* JSONValue_CreateBool(const bool value) { return SEA_JSONValue_CreateBool(value); }
static inline JSONValue* JSONValue_CreateNumber(const double value, Allocator* allocator) { return SEA_JSONValue_CreateNumber(value, allocator); }
static inline JSONValue* JSONValue_CreateString(const char* value, Allocator* allocator) { return SEA_JSONValue_CreateString(value, allocator); }
static inline JSONValue* JSONValue_CreateArray(Allocator* allocator) { return SEA_JSONValue_CreateArray(allocator); }
static inline JSONValue* JSONValue_CreateObject(Allocator* allocator) { return SEA_JSONValue_CreateObject(allocator); }

// =========================
// Instance
static inline char* JSONValue_toString(const JSONValue* self, Allocator* allocator) { return SEA_JSONValue_toString(self, allocator); }
static inline void JSONValue_free(JSONValue* self, Allocator* allocator) { SEA_JSONValue_free(self, allocator); }

#endif

#endif//SEA_JSON_VALUE_H
