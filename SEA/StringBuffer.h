#ifndef SEA_STRING_H
#define SEA_STRING_H

#include "Allocator.h"

#include <stddef.h>

#define SEA_STRING_BUFFER_INITIAL_CAPACITY 64
#define SEA_STRING_BUFFER_INCREASE_FACTOR 4/2 // integer math

struct SeaStringBuffer {
	char* data;
	size_t size;
	size_t pos;
	struct SEA_Allocator* allocator;
};

extern const struct SeaStringBuffer_CLS {
	void (*init)(struct SeaStringBuffer* buffer, size_t capacity, struct SEA_Allocator* allocator);
	void (*append)(struct SeaStringBuffer* self, const char* str);
	char* (*toString)(const struct SeaStringBuffer* self);
} SeaStringBuffer;

#endif//SEA_STRING_H
