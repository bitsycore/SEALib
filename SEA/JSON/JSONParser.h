#ifndef SEA_JSON_PARSER_H
#define SEA_JSON_PARSER_H

#include <stddef.h>

typedef struct {
	const char* json;
	size_t pos;
	size_t len;
	struct SEA_Allocator* allocator;
} SEA_JSONParser;

struct SEA_JSONValue* JSONParser_FromString(const char* string, size_t len, struct SEA_Allocator* allocator);

#endif//SEA_JSON_PARSER_H