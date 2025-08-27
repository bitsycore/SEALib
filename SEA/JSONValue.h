#ifndef SEA_JSON_VALUE_H
#define SEA_JSON_VALUE_H

#include "Config/CompConfig.h"
#include "JSONType.h"
#include "Allocator.h"

#include <stdbool.h>
#include <stdint.h>

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

struct SEA_JSONValue* SEA_JSONValue_FromString(const char* string, size_t len, struct SEA_Allocator* allocator);
struct SEA_JSONValue* SEA_JSONValue_CreateNull();
struct SEA_JSONValue* SEA_JSONValue_CreateBool(bool value);
struct SEA_JSONValue* SEA_JSONValue_CreateNumber(double value, struct SEA_Allocator* allocator);
struct SEA_JSONValue* SEA_JSONValue_CreateString(const char* value, struct SEA_Allocator* allocator);
struct SEA_JSONValue* SEA_JSONValue_CreateArray(struct SEA_Allocator* allocator);
struct SEA_JSONValue* SEA_JSONValue_CreateObject(struct SEA_Allocator* allocator);

// =======================================
// MARK: Instance
// =======================================

char* SEA_JSONValue_toString(const struct SEA_JSONValue* self, struct SEA_Allocator* allocator);
void SEA_JSONValue_free(struct SEA_JSONValue* self, struct SEA_Allocator* allocator);

// =======================================
// MARK: Alias
// =======================================

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

typedef SEA_JSONValue JSONValue;

// Static functions
static inline JSONValue* JSONValue_FromString(const char* string, size_t len, Allocator* allocator) {
	return SEA_JSONValue_FromString(string, len, allocator);
}

static inline JSONValue* JSONValue_CreateNull() {
	return SEA_JSONValue_CreateNull();
}

static inline JSONValue* JSONValue_CreateBool(bool value) {
	return SEA_JSONValue_CreateBool(value);
}

static inline JSONValue* JSONValue_CreateNumber(double value, Allocator* allocator) {
	return SEA_JSONValue_CreateNumber(value, allocator);
}

static inline JSONValue* JSONValue_CreateString(const char* value, Allocator* allocator) {
	return SEA_JSONValue_CreateString(value, allocator);
}

static inline JSONValue* JSONValue_CreateArray(Allocator* allocator) {
	return SEA_JSONValue_CreateArray(allocator);
}

static inline JSONValue* JSONValue_CreateObject(Allocator* allocator) {
	return SEA_JSONValue_CreateObject(allocator);
}

// Instance functions
static inline char* JSONValue_toString(const JSONValue* self, Allocator* allocator) {
	return SEA_JSONValue_toString(self, allocator);
}

static inline void JSONValue_free(JSONValue* self, Allocator* allocator) {
	SEA_JSONValue_free(self, allocator);
}

#endif

#endif//SEA_JSON_VALUE_H