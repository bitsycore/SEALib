#include "String.h"

#include <stddef.h>
#include <string.h>

static void StringBuffer_append(struct SeaStringBuffer* buffer, const char* str) {
	const size_t str_len = strlen(str);
	while (buffer->pos + str_len >= buffer->size) {
		const size_t new_size = buffer->size * 2;
		char* new_data = buffer->allocator->alloc(buffer->allocator->context, new_size);
		if (!new_data) return;
		memcpy(new_data, buffer->data, buffer->pos);
		if (buffer->allocator->free) {
			buffer->allocator->free(buffer->allocator->context, buffer->data);
		}
		buffer->data = new_data;
		buffer->size = new_size;
	}
	strcpy(buffer->data + buffer->pos, str);
	buffer->pos += str_len;
}

static char* StringBuffer_toString(const struct SeaStringBuffer* buffer) {
	return buffer->data;
}

const struct SeaStringBuffer_CLS SeaStringBuffer = {
	.append = StringBuffer_append,
	.toString = StringBuffer_toString,
};