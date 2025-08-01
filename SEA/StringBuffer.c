#include "StringBuffer.h"

#include <stddef.h>
#include <string.h>

static void StringBuffer_init(struct SeaStringBuffer* buffer, size_t capacity, struct SEA_Allocator* allocator) {
	if (buffer->data) return;
	if (capacity == 0) capacity = SEA_STRING_BUFFER_INITIAL_CAPACITY;
	buffer->allocator = allocator ? allocator : SEA_Allocator.Malloc;
	buffer->data = SEA_Allocator.alloc(buffer->allocator, capacity);
	memset(buffer->data, 0, capacity);
	if (!buffer->data) return;
	buffer->size = capacity;
	buffer->pos = 0;
}

static void StringBuffer_append(struct SeaStringBuffer* buffer, const char* str) {
	const size_t str_len = strlen(str);

	if (!buffer->data) {
		StringBuffer_init(buffer, 0, 0);
	}

	while (buffer->pos + str_len >= buffer->size) {
		const size_t new_size = buffer->size * SEA_STRING_BUFFER_INCREASE_FACTOR;
		char* new_data = buffer->allocator->alloc(buffer->allocator->context, new_size);
		if (!new_data) return;
		memcpy(new_data, buffer->data, buffer->pos);
		SEA_Allocator.free(buffer->allocator, buffer->data);
		buffer->data = new_data;
		buffer->size = new_size;
	}

	memcpy(buffer->data + buffer->pos, str, str_len);
	buffer->pos += str_len;
	buffer->data[buffer->pos] = '\0';
}



static char* StringBuffer_toString(const struct SeaStringBuffer* buffer) {
	return buffer->data;
}

const struct SeaStringBuffer_CLS SeaStringBuffer = {
	.append = StringBuffer_append,
	.init = StringBuffer_init,
	.toString = StringBuffer_toString,
};