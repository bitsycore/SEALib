#ifndef SEA_JSON_PARSER_H
#define SEA_JSON_PARSER_H

#include "JSONValue.h"

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

#endif//SEA_JSON_PARSER_H