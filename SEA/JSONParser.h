#ifndef SEA_JSON_PARSER_H
#define SEA_JSON_PARSER_H

#include <stddef.h>

struct SEA_JSONParser {
	const char* json;
	size_t pos;
	size_t len;
	struct SEA_Allocator* allocator;
};

extern const struct SEA_JSONParser_CLS {
	struct SEA_JSONValue* (*FromString)(const char* string, size_t len, struct SEA_Allocator* allocator);
} SEA_JSONParser;

#endif//SEA_JSON_PARSER_H