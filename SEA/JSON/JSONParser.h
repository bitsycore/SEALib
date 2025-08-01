#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <stddef.h>

typedef struct {
	const char* json;
	size_t pos;
	size_t len;
	struct SEA_Allocator* allocator;
} SEA_JSONParser;

struct SEA_JSONValue* JSONParser_FromString(const char* string, const size_t len, struct SEA_Allocator* allocator);

#endif //JSONPARSER_H