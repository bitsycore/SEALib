#ifndef SEA_JSON_PARSER_H
#define SEA_JSON_PARSER_H

#include "Config/CompConfig.h"
#include "JSONValue.h"

#include <stddef.h>

// =======================================
// MARK: Types
// =======================================

typedef struct SEA_JSONParser {
	const char* json;
	size_t pos;
	size_t len;
	struct SEA_Allocator* allocator;
} SEA_JSONParser;

// =======================================
// MARK: Static
// =======================================

struct SEA_JSONValue* SEA_JSONParser_FromString(const char* string, size_t len, struct SEA_Allocator* allocator);

// =======================================
// MARK: Alias
// =======================================

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

typedef SEA_JSONParser JSONParser;

static inline JSONValue* JSONParser_FromString(const char* string, size_t len, Allocator* allocator) {
	return SEA_JSONParser_FromString(string, len, allocator);
}

#endif

#endif//SEA_JSON_PARSER_H