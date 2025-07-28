#ifndef SEA_STRING_H
#define SEA_STRING_H

#include "Allocator.h"

#include <stddef.h>

struct SeaStringBuffer {
	char* data;
	size_t size;
	size_t pos;
	struct SeaAllocator* allocator;
};

extern const struct SeaStringBuffer_CLS {
	void (*append)(struct SeaStringBuffer* self, const char* str);
	char* (*toString)(const struct SeaStringBuffer* self);
} SeaStringBuffer;

#endif//SEA_STRING_H
