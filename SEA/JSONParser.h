#ifndef SEA_JSON_PARSER_H
#define SEA_JSON_PARSER_H

#include "JSONValue.h"
#include "Config/CompConfig.h"

#include <stddef.h>

// =======================================
// MARK: Types
// =======================================

typedef struct SEA_JSONParser {
	const char* json;
	size_t pos;
	size_t len;
	SEA_Allocator* allocator;
} SEA_JSONParser;

// =======================================
// MARK: Static
// =======================================

SEA_JSONValue* SEA_JSONParser_FromString(const char* string, size_t len, SEA_Allocator* allocator);

// =======================================
// MARK: Alias
// =======================================

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

// =========================
// Types
typedef SEA_JSONParser JSONParser;

// =========================
// Static
static inline JSONValue* JSONParser_FromString(const char* string, const size_t len, Allocator* allocator) { return SEA_JSONParser_FromString(string, len, allocator); }

#endif

#endif//SEA_JSON_PARSER_H