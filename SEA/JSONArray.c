#include "JSONArray.h"

#include "JSONObject.h"
#include "JSONValue.h"
#include "Time.h"

// ===================================
// MARK: JsonArray
// ===================================

struct SEA_JSONValue* SEA_JSONArray_New(struct SEA_Allocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	uint8_t* buffer = SEA_Allocator_alloc(
		alloc,
		sizeof(struct SEA_JSONValue) + sizeof(struct SEA_JSONArray)
	);
	if (!buffer) return NULL;

	struct SEA_JSONValue* value = (struct SEA_JSONValue*)buffer;
	value->type = SEA_JSON_ARRAY;
	value->array = (struct SEA_JSONArray*)(buffer + sizeof(struct SEA_JSONValue));
	value->array->items = NULL;
	value->array->count = 0;
	value->array->capacity = 0;
	value->array->ref_count = 1;

	return value;
}

void SEA_JSONArray_add(struct SEA_JSONArray* self, struct SEA_JSONValue* value, struct SEA_Allocator* allocator) {
	if (!self || !value || !allocator || !allocator->alloc) return;

	if (self->count >= self->capacity) {
		const size_t new_capacity = self->capacity == 0 ? SEA_JSON_ARRAY_INITIAL_CAPACITY : self->capacity * 2;
		const size_t new_size = new_capacity * sizeof(struct SEA_JSONValue*);

		struct SEA_JSONValue** new_items = SEA_Allocator_alloc(allocator, new_size);
		if (!new_items) return;

		// Copy old items
		if (self->items) {
			for (size_t i = 0; i < self->count; i++) {
				new_items[i] = self->items[i];
			}
			SEA_Allocator_free(allocator, self->items);
		}

		self->items = new_items;
		self->capacity = new_capacity;
	}

	self->items[self->count++] = value;
}

struct SEA_JSONValue* SEA_JSONArray_get(const struct SEA_JSONArray* array, const size_t index) {
	if (!array || index >= array->count) return NULL;
	struct SEA_JSONValue* val = array->items[index];
	if (val->type == SEA_JSON_OBJECT) {
		val->object->ref_count++;
	}
	if (val->type == SEA_JSON_ARRAY) {
		val->array->ref_count++;
	}
	return val;
}

void SEA_JSONArray_free(struct SEA_JSONValue * self, struct SEA_Allocator * alloc) {
	self->array->ref_count--;
	if (self->array->ref_count > 0) return;
	for (size_t i = 0; i < self->array->count; i++) {
		SEA_JSONValue_free(self->array->items[i], alloc);
	}
	SEA_Allocator_free(alloc, self->array->items);
	SEA_Allocator_free(alloc, self);
}

struct SEA_JSONValue* SEA_JSONArray_asJSONValue(const struct SEA_JSONArray* self) {
	if (!self) return NULL;
	return (struct SEA_JSONValue*)((char*)self - sizeof(struct SEA_JSONValue));
}