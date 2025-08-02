#include "JSONArray.h"

#include "JSONObject.h"
#include "JSONValue.h"
#include "Time.h"

// ===================================
// MARK: JsonArray
// ===================================

static struct SEA_JSONValue* JSONArray_New(struct SEA_Allocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	uint8_t* buffer = SEA_Allocator.alloc(
		alloc,
		sizeof(struct SEA_JSONValue) + sizeof(struct SEA_JSONArray)
	);
	if (!buffer) return NULL;

	struct SEA_JSONValue* value = (struct SEA_JSONValue*)buffer;
	value->type = SEA_JSON_ARRAY;
	value->timestamp = SEA_Time.getMillis();
	value->array = (struct SEA_JSONArray*)(buffer + sizeof(struct SEA_JSONValue));
	value->array->items = NULL;
	value->array->count = 0;
	value->array->capacity = 0;
	value->array->ref_count = 1;

	return value;
}

static void JSONArray_add(struct SEA_JSONArray* self, struct SEA_JSONValue* value, struct SEA_Allocator* allocator) {
	if (!self || !value || !allocator || !allocator->alloc) return;

	if (self->count >= self->capacity) {
		const size_t new_capacity = self->capacity == 0 ? SEA_JSON_ARRAY_INITIAL_CAPACITY : self->capacity * 2;
		const size_t new_size = new_capacity * sizeof(struct SEA_JSONValue*);

		struct SEA_JSONValue** new_items = SEA_Allocator.alloc(allocator, new_size);
		if (!new_items) return;

		// Copy old items
		if (self->items) {
			for (size_t i = 0; i < self->count; i++) {
				new_items[i] = self->items[i];
			}
			SEA_Allocator.free(allocator, self->items);
		}

		self->items = new_items;
		self->capacity = new_capacity;
	}

	self->items[self->count++] = value;
}

static struct SEA_JSONValue* JSONArray_get(const struct SEA_JSONArray* array, const size_t index) {
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

static void JSONArray_free(struct SEA_JSONValue * self, struct SEA_Allocator * alloc) {
	self->array->ref_count--;
	if (self->array->ref_count > 0) return;
	for (size_t i = 0; i < self->array->count; i++) {
		SEA_JSONValue.free(self->array->items[i], alloc);
	}
	SEA_Allocator.free(alloc, self->array->items);
	SEA_Allocator.free(alloc, self);
}

static struct SEA_JSONValue* JSONArray_toJSONValue(const struct SEA_JSONArray* self) {
	if (!self) return NULL;
	return (struct SEA_JSONValue*)((char*)self - sizeof(struct SEA_JSONValue));
}

const struct SEA_JSONArray_CLS SEA_JSONArray = {
	.New = JSONArray_New,
	.add = JSONArray_add,
	.get = JSONArray_get,
	.free = JSONArray_free,
	.toJSONValue = JSONArray_toJSONValue,
};
