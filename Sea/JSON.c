#include "JSON.h"

#include <ctype.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Allocator.h"
#include "Compat.h"
#include "Error.h"
#include "String.h"

// ===================================
// MARK: Internal Types
// ===================================

typedef struct {
	const char* json;
	size_t pos;
	size_t len;
	struct SeaAllocator* allocator;
} InternalJsonParser;

typedef struct SeaJsonObjectEntry {
	char*                       key;
	size_t                      key_len;
	size_t                      key_hash;
	struct SeaJsonValue*        value;
	struct SeaJsonObjectEntry*  next;
} InternalJsonObjectEntry;

static struct SeaJsonValue InternalNullSingleton = {
	.type = SEA_JSON_NULL,
	.string = NULL,
};

// ===================================
// MARK: Forward
// ===================================

static struct SeaJsonValue* ParseValue(InternalJsonParser* parser);
static void JsonValue_free(struct SeaJsonValue* self, struct SeaAllocator* alloc);
void JsonArray_add(struct SeaJsonArray* self, struct SeaJsonValue* value, struct SeaAllocator* alloc);
enum SeaErrorType JsonObject_put(struct SeaJsonObject* self, const char* key, struct SeaJsonValue* value, struct SeaAllocator* alloc);

// ===================================
// MARK: Utility
// ===================================

static void SkipWhitespace(InternalJsonParser* parser) {
	while (parser->pos < parser->len && isspace(parser->json[parser->pos])) {
		parser->pos++;
	}
}

static size_t HashString(const char* s) {
	size_t h = 5381;
	while (*s) {
		h = ((h << 5) + h) + (unsigned char)*s++;
	}
	return h;
}

// ===================================
// MARK: Create
// ===================================

struct SeaJsonValue* CreateNull() {
	return &InternalNullSingleton;
}

struct SeaJsonValue* CreateBool(const bool val, struct SeaAllocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	struct SeaJsonValue* value = alloc->alloc(alloc->context, sizeof(struct SeaJsonValue), sizeof(void*));
	if (!value) return NULL;
	value->type = SEA_JSON_BOOL;
	value->boolean = val;

	return value;
}

struct SeaJsonValue* CreateNumber(const double val, struct SeaAllocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	struct SeaJsonValue* value = alloc->alloc(alloc->context, sizeof(struct SeaJsonValue), sizeof(void*));
	if (!value) return NULL;
	value->type = SEA_JSON_NUMBER;
	value->number = val;

	return value;
}

struct SeaJsonValue* CreateString(const char* val, struct SeaAllocator* alloc) {
	if (!alloc || !alloc->alloc || !val) return NULL;

	const size_t bufferLen = sizeof(struct SeaJsonValue) + strlen(val) + 1;
	uint8_t* buffer = alloc->alloc(alloc->context, bufferLen, 0);
	if (!buffer) return NULL;
	struct SeaJsonValue* value = (struct SeaJsonValue*) buffer;
	value->string = (char*) (buffer + sizeof(struct SeaJsonValue));
	memcpy(value->string, val, strlen(val) + 1);
	value->type = SEA_JSON_STRING;

	if (!value->string) {
		if (alloc->free) alloc->free(alloc->context, value, sizeof(struct SeaJsonValue));
		return NULL;
	}

	return value;
}

struct SeaJsonValue* CreateArray(struct SeaAllocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	uint8_t* buffer = alloc->alloc(
		alloc->context,
		sizeof(struct SeaJsonValue)
			+ sizeof(struct SeaJsonArray),
		0
	);
	if (!buffer) return NULL;

	struct SeaJsonValue* value = (struct SeaJsonValue*) buffer;
	value->type = SEA_JSON_ARRAY;
	value->array = (struct SeaJsonArray*) (buffer + sizeof(struct SeaJsonValue));
	value->array->items = NULL;
	value->array->count = 0;
	value->array->capacity = 0;
	value->array->ref_count = 1;

	return value;
}

struct SeaJsonValue* CreateObject(struct SeaAllocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	uint8_t* buffer = alloc->alloc(
		alloc->context,
		sizeof(struct SeaJsonValue)
			+ sizeof(struct SeaJsonObject)
			+ sizeof(InternalJsonObjectEntry) * SEA_JSON_OBJECT_BUCKETS_NUMBERS,
		0
	);
	if (!buffer) return NULL;

	struct SeaJsonValue* value = (struct SeaJsonValue*) buffer;
	value->object = (struct SeaJsonObject*) (buffer + sizeof(struct SeaJsonValue));
	value->object->buckets = (void**) (buffer + sizeof(struct SeaJsonValue) + sizeof(struct SeaJsonObject));
	memset(value->object->buckets, 0, SEA_JSON_OBJECT_BUCKETS_NUMBERS * sizeof(InternalJsonObjectEntry*));

	value->type = SEA_JSON_OBJECT;
	value->object->bucketCount = SEA_JSON_OBJECT_BUCKETS_NUMBERS;
	value->object->size = 0;
	value->object->ref_count = 1;

	return value;
}

// ===================================
// MARK: Parse
// ===================================

static bool ParseLiteral(InternalJsonParser* parser, const char* literal) {
	const size_t len = strlen(literal);
	if (parser->pos + len > parser->len) return false;
	if (strncmp(parser->json + parser->pos, literal, len) != 0) return false;
	parser->pos += len;
	return true;
}

static struct SeaJsonValue* ParseString(InternalJsonParser* parser) {
	if (parser->pos >= parser->len || parser->json[parser->pos] != '"') return NULL;
	parser->pos++; // Skip opening quote

	const size_t start = parser->pos;
	while (parser->pos < parser->len && parser->json[parser->pos] != '"') {
		if (parser->json[parser->pos] == '\\' && parser->pos + 1 < parser->len) {
			parser->pos += 2; // Skip escape sequence
		}
		else {
			parser->pos++;
		}
	}

	if (parser->pos >= parser->len) return NULL; // Unterminated string

	const size_t len = parser->pos - start;
	char* str = parser->allocator->alloc(parser->allocator->context, len + 1, 1);
	if (!str) return NULL;
	strncpy(str, parser->json + start, len);
	str[len] = '\0';
	parser->pos++; // Skip closing quote

	struct SeaJsonValue* value = CreateString(str, parser->allocator);
	if (parser->allocator->free) {
		parser->allocator->free(parser->allocator->context, str, len + 1);
	}
	return value;
}

static struct SeaJsonValue* ParseNumber(InternalJsonParser* parser) {
	const size_t start = parser->pos;

	if (parser->json[parser->pos] == '-') parser->pos++;

	if (!isdigit(parser->json[parser->pos])) return NULL;

	while (parser->pos < parser->len && isdigit(parser->json[parser->pos])) {
		parser->pos++;
	}

	if (parser->pos < parser->len && parser->json[parser->pos] == '.') {
		parser->pos++;
		if (!isdigit(parser->json[parser->pos])) return NULL;
		while (parser->pos < parser->len && isdigit(parser->json[parser->pos])) {
			parser->pos++;
		}
	}

	if (parser->pos < parser->len && (parser->json[parser->pos] == 'e' || parser->json[parser->pos] == 'E')) {
		parser->pos++;
		if (parser->json[parser->pos] == '+' || parser->json[parser->pos] == '-') parser->pos++;
		if (!isdigit(parser->json[parser->pos])) return NULL;
		while (parser->pos < parser->len && isdigit(parser->json[parser->pos])) {
			parser->pos++;
		}
	}

	const size_t len = parser->pos - start;
	char* num_str = parser->allocator->alloc(parser->allocator->context, len + 1, 1);
	if (!num_str) return NULL;
	strncpy(num_str, parser->json + start, len);
	num_str[len] = '\0';

	const double num = atof(num_str);
	if (parser->allocator->free) {
		parser->allocator->free(parser->allocator->context, num_str, len + 1);
	}

	return CreateNumber(num, parser->allocator);
}

static struct SeaJsonValue* ParseArray(InternalJsonParser* parser) {
	if (parser->pos >= parser->len || parser->json[parser->pos] != '[') return NULL;
	parser->pos++; // Skip '['

	struct SeaJsonValue* array = CreateArray(parser->allocator);
	if (!array) return NULL;

	SkipWhitespace(parser);

	// Empty array case
	if (parser->pos < parser->len && parser->json[parser->pos] == ']') {
		parser->pos++;
		return array;
	}

	while (parser->pos < parser->len) {
		struct SeaJsonValue* value = ParseValue(parser);
		if (!value) {
			JsonValue_free(array, parser->allocator);
			return NULL;
		}

		JsonArray_add(array->array, value, parser->allocator);

		SkipWhitespace(parser);

		if (parser->pos >= parser->len) break;

		if (parser->json[parser->pos] == ']') {
			parser->pos++;
			return array;
		}

		if (parser->json[parser->pos] != ',') {
			JsonValue_free(array, parser->allocator);
			return NULL;
		}

		parser->pos++; // Skip ','
		SkipWhitespace(parser);
	}

	JsonValue_free(array, parser->allocator);
	return NULL;
}

static struct SeaJsonValue* ParseObject(InternalJsonParser* parser) {
	if (parser->pos >= parser->len || parser->json[parser->pos] != '{') return NULL;
	parser->pos++; // Skip '{'

	struct SeaJsonValue* object = CreateObject(parser->allocator);
	if (!object) return NULL;

	SkipWhitespace(parser);

	if (parser->pos < parser->len && parser->json[parser->pos] == '}') {
		parser->pos++;
		return object;
	}

	while (parser->pos < parser->len) {
		SkipWhitespace(parser);

		struct SeaJsonValue* key = ParseString(parser);
		if (!key) {
			JsonValue_free(object, parser->allocator);
			return NULL;
		}

		SkipWhitespace(parser);

		if (parser->pos >= parser->len || parser->json[parser->pos] != ':') {
			JsonValue_free(key, parser->allocator);
			JsonValue_free(object, parser->allocator);
			return NULL;
		}

		parser->pos++; // Skip ':'
		SkipWhitespace(parser);

		struct SeaJsonValue* value = ParseValue(parser);
		if (!value) {
			JsonValue_free(key, parser->allocator);
			JsonValue_free(object, parser->allocator);
			return NULL;
		}

		JsonObject_put(object->object, key->string, value, parser->allocator);
		JsonValue_free(key, parser->allocator);

		SkipWhitespace(parser);

		if (parser->pos >= parser->len) break;

		if (parser->json[parser->pos] == '}') {
			parser->pos++;
			return object;
		}

		if (parser->json[parser->pos] != ',') {
			JsonValue_free(object, parser->allocator);
			return NULL;
		}

		parser->pos++; // Skip ','
	}

	JsonValue_free(object, parser->allocator);
	return NULL;
}

static struct SeaJsonValue* ParseValue(InternalJsonParser* parser) {
	SkipWhitespace(parser);

	if (parser->pos >= parser->len)
		return NULL;

	const char c = parser->json[parser->pos];

	switch (c) {
	case '"':
		return ParseString(parser);
	case '[':
		return ParseArray(parser);
	case '{':
		return ParseObject(parser);
	case 't':
		if (ParseLiteral(parser, "true")) {
			return CreateBool(true, parser->allocator);
		}
		return NULL;
	case 'f':
		if (ParseLiteral(parser, "false")) {
			return CreateBool(false, parser->allocator);
		}
		return NULL;
	case 'n':
		if (ParseLiteral(parser, "null")) {
			return CreateNull();
		}
		return NULL;
	default:
		if (c == '-' || isdigit(c)) {
			return ParseNumber(parser);
		}
		return NULL;
	}
}

// =========================================
// MARK: Memory cleanup
// =========================================

static void Stringify(const struct SeaJsonValue* value, struct SeaStringBuffer* buffer) {
	if (!value) return;

	switch (value->type) {
	case SEA_JSON_NULL:
		SeaStringBuffer.append(buffer, "null");
		break;
	case SEA_JSON_BOOL:
		SeaStringBuffer.append(buffer, value->boolean ? "true" : "false");
		break;
	case SEA_JSON_NUMBER: {
		char num_str[32];
		snprintf(num_str, sizeof(num_str), "%.15g", value->number);
		SeaStringBuffer.append(buffer, num_str);
		break;
	}
	case SEA_JSON_STRING:
		SeaStringBuffer.append(buffer, "\"");
		SeaStringBuffer.append(buffer, value->string);
		SeaStringBuffer.append(buffer, "\"");
		break;
	case SEA_JSON_ARRAY:
		SeaStringBuffer.append(buffer, "[");
		for (size_t i = 0; i < value->array->count; i++) {
			if (i > 0) SeaStringBuffer.append(buffer, ",");
			Stringify(value->array->items[i], buffer);
		}
		SeaStringBuffer.append(buffer, "]");
		break;
		case SEA_JSON_OBJECT: {
			SeaStringBuffer.append(buffer, "{");
			bool first = true;
			const struct SeaJsonObject* obj = value->object;
			for (size_t bi = 0; bi < obj->bucketCount; ++bi) {
				const InternalJsonObjectEntry* e = obj->buckets[bi];
				while (e) {
					if (!first) {
						SeaStringBuffer.append(buffer, ",");
					}
					first = false;
					SeaStringBuffer.append(buffer, "\"");
					SeaStringBuffer.append(buffer, e->key);
					SeaStringBuffer.append(buffer, "\":");
					Stringify(e->value, buffer);
					e = e->next;
				}
			}
			SeaStringBuffer.append(buffer, "}");
			break;
		}
	}
}

// =========================================
// MARK: JsonValue
// =========================================

static struct SeaJsonValue* ParseJson(const char* string, const size_t len, struct SeaAllocator* allocator) {
	if (!string) return NULL;

	if (!allocator || !allocator->alloc) allocator = SeaAllocator.Malloc;

	InternalJsonParser parser = {
		.json = string,
		.pos = 0,
		.len = len == 0 ? strlen(string) : len,
		.allocator = allocator
	};

	return ParseValue(&parser);
}

static char* JsonValue_toString(const struct SeaJsonValue* self, struct SeaAllocator* alloc) {
	if (!self || !alloc || !alloc->alloc) return NULL;

	struct SeaStringBuffer buffer = {
		.data = (char*)alloc->alloc(alloc->context, 256, SEA_ALIGNOF(char)),
		.size = 256,
		.pos = 0,
		.allocator = alloc
	};

	if (!buffer.data) return NULL;
	buffer.data[0] = '\0';

	Stringify(self, &buffer);

	return buffer.data;
}

void FreeArray(struct SeaJsonValue * self, struct SeaAllocator * alloc) {
	if (self->array->ref_count > 1) {
		self->array->ref_count--;
		return;
	}
	for (size_t i = 0; i < self->array->count; i++) {
		JsonValue_free(self->array->items[i], alloc);
	}
	if (alloc->free && self->array->items) {
		alloc->free(alloc->context, self->array->items,self->array->capacity * sizeof(struct SeaJsonValue*));
	}
	if (alloc->free) {
		alloc->free(alloc->context, self, sizeof(struct SeaJsonValue) + sizeof(struct SeaJsonArray));
	}
}

void FreeObject(struct SeaJsonValue * self, struct SeaAllocator * alloc) {
	if (self->object->ref_count > 1) {
		self->object->ref_count--;
		return;
	}

	for (size_t i = 0; i < self->object->bucketCount; i++) {
		const InternalJsonObjectEntry* next = self->object->buckets[i];
		for (const InternalJsonObjectEntry* e = next; e != NULL; e = next) {
			next = e->next;
			// Free value
			JsonValue_free(e->value, alloc);
			if (alloc->free) {
				// Key
				alloc->free(alloc->context, e->key, e->key_len + 1);
			}
		}
	}

	if (alloc->free) {
		alloc->free(alloc->context, self, sizeof(struct SeaJsonValue) + sizeof(struct SeaJsonObject) + self->object->bucketCount * sizeof(InternalJsonObjectEntry*));
	}
}

static void JsonValue_free(struct SeaJsonValue *self, struct SeaAllocator *alloc) {
	if (!self) return;
	switch (self->type) {
		case SEA_JSON_ARRAY: FreeArray(self, alloc);
			return;
		case SEA_JSON_OBJECT: FreeObject(self, alloc);
			return;
		default:
			if (self->type != SEA_JSON_NULL && alloc->free) {
				alloc->free(alloc->context, self, sizeof(struct SeaJsonValue));
			}
	}
}

const struct SeaJsonValue_CLS SeaJsonValue = {
	// Static
	.ParseJson = ParseJson,
	.CreateNull = CreateNull,
	.CreateBool = CreateBool,
	.CreateNumber = CreateNumber,
	.CreateString = CreateString,
	.CreateArray = CreateArray,
	.CreateObject = CreateObject,

	// Instance
	.toString = JsonValue_toString,
	.free = JsonValue_free,
};

// ===================================
// MARK: JsonObject
// ===================================

enum SeaErrorType JsonObject_put(struct SeaJsonObject* self, const char* key, struct SeaJsonValue* value, struct SeaAllocator* alloc) {
	if (!self ) {
		SeaError.setError(SEA_ERROR_GENERIC_SELF_NULL);
		return SEA_ERROR_GENERIC_SELF_NULL;
	}

	if (!key || !value || !alloc || !alloc->alloc) {
		SeaError.setError(SEA_ERROR_GENERIC_ARGUMENT_NULL);
		return SEA_ERROR_GENERIC_ARGUMENT_NULL;
	}

	if (!alloc->alloc) {
		SeaError.setError(SEA_ERROR_ALLOCATOR_ALLOC_FUNC_NULL);
		return SEA_ERROR_ALLOCATOR_ALLOC_FUNC_NULL;
	}

	const size_t idx = HashString(key) % self->bucketCount;
	InternalJsonObjectEntry** bucket = (InternalJsonObjectEntry**) &self->buckets[idx];

	// Check if the key already exists
	// search for an existing key
	for (InternalJsonObjectEntry* e = *bucket; e != NULL; e = e->next) {
		if (strcmp(e->key, key) == 0) {
			JsonValue_free(e->value, alloc);
			e->value = value;
			return SEA_ERROR_NONE;
		}
	}

	InternalJsonObjectEntry* ne = alloc->alloc(alloc->context, sizeof(*ne), SEA_ALIGNOF(InternalJsonObjectEntry*));
	ne->key = SeaAllocator.strdup(alloc, key);
	ne->key_len = strlen(key);
	ne->value = value;
	ne->next  = *bucket;
	*bucket = ne;
	self->size++;

	return SEA_ERROR_NONE;
}

struct SeaJsonValue* JsonObject_get(const struct SeaJsonObject* self, const char* key)
{
	const size_t idx = HashString(key) % self->bucketCount;
	for (const InternalJsonObjectEntry* e = self->buckets[idx]; e; e = e->next) {
		if (strcmp(e->key, key) == 0) {
			return e->value;
		}
	}
	return NULL;
}


bool JsonObject_has(const struct SeaJsonObject* self, const char* key) {
	if (!self || !key) return false;
	const size_t idx = HashString(key) % self->bucketCount;
	for (const InternalJsonObjectEntry* e = self->buckets[idx]; e; e = e->next) {
		if (strcmp(e->key, key) == 0) return true;
	}

	return false;
}

size_t JsonObject_size(const struct SeaJsonObject* self) {
	if (!self) return 0;
	return self->size;
}

bool JsonObject_remove(struct SeaJsonObject* self, const char* key, struct SeaAllocator* alloc) {
	if (!self || !key || !alloc ) return false;

	const size_t idx = HashString(key) % self->bucketCount;
	InternalJsonObjectEntry** prev = (InternalJsonObjectEntry**) &self->buckets[idx];
	for (InternalJsonObjectEntry* e = *prev; e; e = e->next) {
		if (strcmp(e->key, key) == 0) {
			*prev = e->next;
			alloc->free(alloc->context, e->key, e->key_len + 1);
			JsonValue_free(e->value, alloc);
			self->size--;
			return true;
		}
		prev = &e->next;
	}
	return false;
}

const struct SeaJsonObject_CLS SeaJsonObject = {
	.put = JsonObject_put,
	.get = JsonObject_get,
	.has = JsonObject_has,
	.size = JsonObject_size,
	.remove = JsonObject_remove,
};

// ===================================
// MARK: JsonArray
// ===================================

void JsonArray_add(struct SeaJsonArray* self, struct SeaJsonValue* value, struct SeaAllocator* alloc) {
	if (!self || !value || !alloc || !alloc->alloc) return;

	if (self->count >= self->capacity) {
		const size_t new_capacity = self->capacity == 0 ? SEA_JSON_ARRAY_INITIAL_CAPACITY : self->capacity * 2;
		const size_t new_size = new_capacity * sizeof(struct SeaJsonValue*);

		struct SeaJsonValue** new_items = alloc->alloc(alloc->context, new_size, sizeof(void*));
		if (!new_items) return;

		// Copy old items
		if (self->items) {
			for (size_t i = 0; i < self->count; i++) {
				new_items[i] = self->items[i];
			}
			if (alloc->free) {
				alloc->free(alloc->context, self->items, self->capacity * sizeof(struct SeaJsonValue*));
			}
		}

		self->items = new_items;
		self->capacity = new_capacity;
	}

	self->items[self->count++] = value;
}

struct SeaJsonValue* JsonArray_get(const struct SeaJsonArray* array, const size_t index) {
	if (!array || index >= array->count) return NULL;
	struct SeaJsonValue* val = array->items[index];
	if (val->type == SEA_JSON_OBJECT) {
		val->object->ref_count++;
	}
	if (val->type == SEA_JSON_ARRAY) {
		val->array->ref_count++;
	}
	return val;
}

const struct SeaJsonArray_CLS SeaJsonArray = {
	.add = JsonArray_add,
	.get = JsonArray_get,
};
