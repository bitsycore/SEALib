#include "JSON.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "Allocator.h"
#include "String.h"

// ===================================
// MARK: Internal Types
// ===================================

typedef struct {
	const char* json;
	size_t pos;
	size_t len;
	struct SeaAllocator* alloc;
} InternalJsonParser;

// ===================================
// MARK: Forward
// ===================================

void JsonArray_add(struct SeaJsonValue* array, struct SeaJsonValue* value, struct SeaAllocator* alloc);
struct SeaJsonValue* JsonArray_get(const struct SeaJsonValue* array, size_t index);

void JsonObject_put(struct SeaJsonValue* object, const char* key, struct SeaJsonValue* value, struct SeaAllocator* alloc);
struct SeaJsonValue* JsonObject_get(const struct SeaJsonValue* object, const char* key);

static struct SeaJsonValue* Parse_Value(InternalJsonParser* parser);
static void JsonValue_free(struct SeaJsonValue* self, struct SeaAllocator* alloc);

// ===================================
// MARK: Utility
// ===================================

static void Utility_SkipWhitespace(InternalJsonParser* parser) {
	while (parser->pos < parser->len && isspace(parser->json[parser->pos])) {
		parser->pos++;
	}
}

// ===================================
// MARK: Create
// ===================================

struct SeaJsonValue* JsonValue_CreateNull(struct SeaAllocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	struct SeaJsonValue* value = alloc->alloc(alloc->context, sizeof(struct SeaJsonValue), sizeof(void*));
	if (!value) return NULL;
	value->type = SEAJSON_NULL;

	return value;
}

struct SeaJsonValue* JsonValue_CreateBool(const bool val, struct SeaAllocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	struct SeaJsonValue* value = alloc->alloc(alloc->context, sizeof(struct SeaJsonValue), sizeof(void*));
	if (!value) return NULL;
	value->type = SEAJSON_BOOL;
	value->boolean = val;

	return value;
}

struct SeaJsonValue* JsonValue_CreateNumber(const double val, struct SeaAllocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	struct SeaJsonValue* value = alloc->alloc(alloc->context, sizeof(struct SeaJsonValue), sizeof(void*));
	if (!value) return NULL;
	value->type = SEAJSON_NUMBER;
	value->number = val;

	return value;
}

struct SeaJsonValue* JsonValue_CreateString(const char* val, struct SeaAllocator* alloc) {
	if (!alloc || !alloc->alloc || !val) return NULL;

	struct SeaJsonValue* value = alloc->alloc(alloc->context, sizeof(struct SeaJsonValue), sizeof(void*));
	if (!value) return NULL;

	value->type = SEAJSON_STRING;
	value->string = SeaAllocator.strdup(alloc, val);
	if (!value->string) {
		if (alloc->free) alloc->free(alloc->context, value, sizeof(struct SeaJsonValue));
		return NULL;
	}
	return value;
}

struct SeaJsonValue* JsonValue_CreateArray(struct SeaAllocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	struct SeaJsonValue* value = alloc->alloc(alloc->context, sizeof(struct SeaJsonValue), sizeof(void*));
	if (!value) return NULL;
	value->type = SEAJSON_ARRAY;
	value->array.items = NULL;
	value->array.count = 0;
	value->array.capacity = 0;

	return value;
}

struct SeaJsonValue* JsonValue_CreateObject(struct SeaAllocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	struct SeaJsonValue* value = alloc->alloc(alloc->context, sizeof(struct SeaJsonValue), sizeof(void*));
	if (!value) return NULL;
	value->type = SEAJSON_OBJECT;
	value->object.keys = NULL;
	value->object.values = NULL;
	value->object.count = 0;
	value->object.capacity = 0;

	return value;
}

// ===================================
// MARK: Parse
// ===================================

static bool Parse_Literal(InternalJsonParser* parser, const char* literal) {
	const size_t len = strlen(literal);
	if (parser->pos + len > parser->len) return false;
	if (strncmp(parser->json + parser->pos, literal, len) != 0) return false;
	parser->pos += len;
	return true;
}

static struct SeaJsonValue* Parse_String(InternalJsonParser* parser) {
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
	char* str = parser->alloc->alloc(parser->alloc->context, len + 1, 1);
	if (!str) return NULL;
	strncpy(str, parser->json + start, len);
	str[len] = '\0';
	parser->pos++; // Skip closing quote

	struct SeaJsonValue* value = JsonValue_CreateString(str, parser->alloc);
	if (parser->alloc->free) {
		parser->alloc->free(parser->alloc->context, str, len + 1);
	}
	return value;
}

static struct SeaJsonValue* Parse_Number(InternalJsonParser* parser) {
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

	size_t len = parser->pos - start;
	char* num_str = parser->alloc->alloc(parser->alloc->context, len + 1, 1);
	if (!num_str) return NULL;
	strncpy(num_str, parser->json + start, len);
	num_str[len] = '\0';

	const double num = atof(num_str);
	if (parser->alloc->free) {
		parser->alloc->free(parser->alloc->context, num_str, len + 1);
	}

	return JsonValue_CreateNumber(num, parser->alloc);
}

static struct SeaJsonValue* Parse_Array(InternalJsonParser* parser) {
	if (parser->pos >= parser->len || parser->json[parser->pos] != '[') return NULL;
	parser->pos++; // Skip '['

	struct SeaJsonValue* array = JsonValue_CreateArray(parser->alloc);
	if (!array) return NULL;

	Utility_SkipWhitespace(parser);

	if (parser->pos < parser->len && parser->json[parser->pos] == ']') {
		parser->pos++;
		return array; // Empty array
	}

	while (parser->pos < parser->len) {
		struct SeaJsonValue* value = Parse_Value(parser);
		if (!value) {
			JsonValue_free(array, parser->alloc);
			return NULL;
		}

		JsonArray_add(array, value, parser->alloc);

		Utility_SkipWhitespace(parser);

		if (parser->pos >= parser->len) break;

		if (parser->json[parser->pos] == ']') {
			parser->pos++;
			return array;
		}

		if (parser->json[parser->pos] != ',') {
			JsonValue_free(array, parser->alloc);
			return NULL;
		}

		parser->pos++; // Skip ','
		Utility_SkipWhitespace(parser);
	}

	JsonValue_free(array, parser->alloc);
	return NULL;
}

static struct SeaJsonValue* Parse_Object(InternalJsonParser* parser) {
	if (parser->pos >= parser->len || parser->json[parser->pos] != '{') return NULL;
	parser->pos++; // Skip '{'

	struct SeaJsonValue* object = JsonValue_CreateObject(parser->alloc);
	if (!object) return NULL;

	Utility_SkipWhitespace(parser);

	if (parser->pos < parser->len && parser->json[parser->pos] == '}') {
		parser->pos++;
		return object; // Empty object
	}

	while (parser->pos < parser->len) {
		Utility_SkipWhitespace(parser);

		struct SeaJsonValue* key = Parse_String(parser);
		if (!key) {
			JsonValue_free(object, parser->alloc);
			return NULL;
		}

		Utility_SkipWhitespace(parser);

		if (parser->pos >= parser->len || parser->json[parser->pos] != ':') {
			JsonValue_free(key, parser->alloc);
			JsonValue_free(object, parser->alloc);
			return NULL;
		}

		parser->pos++; // Skip ':'
		Utility_SkipWhitespace(parser);

		struct SeaJsonValue* value = Parse_Value(parser);
		if (!value) {
			JsonValue_free(key, parser->alloc);
			JsonValue_free(object, parser->alloc);
			return NULL;
		}

		JsonObject_put(object, key->string, value, parser->alloc);
		JsonValue_free(key, parser->alloc);

		Utility_SkipWhitespace(parser);

		if (parser->pos >= parser->len) break;

		if (parser->json[parser->pos] == '}') {
			parser->pos++;
			return object;
		}

		if (parser->json[parser->pos] != ',') {
			JsonValue_free(object, parser->alloc);
			return NULL;
		}

		parser->pos++; // Skip ','
	}

	JsonValue_free(object, parser->alloc);
	return NULL;
}

static struct SeaJsonValue* Parse_Value(InternalJsonParser* parser) {
	Utility_SkipWhitespace(parser);

	if (parser->pos >= parser->len) return NULL;

	const char c = parser->json[parser->pos];

	switch (c) {
	case '"':
		return Parse_String(parser);
	case '[':
		return Parse_Array(parser);
	case '{':
		return Parse_Object(parser);
	case 't':
		if (Parse_Literal(parser, "true")) {
			return JsonValue_CreateBool(true, parser->alloc);
		}
		return NULL;
	case 'f':
		if (Parse_Literal(parser, "false")) {
			return JsonValue_CreateBool(false, parser->alloc);
		}
		return NULL;
	case 'n':
		if (Parse_Literal(parser, "null")) {
			return JsonValue_CreateNull(parser->alloc);
		}
		return NULL;
	default:
		if (c == '-' || isdigit(c)) {
			return Parse_Number(parser);
		}
		return NULL;
	}
}

// =========================================
// MARK: Memory cleanup
// =========================================

static void JsonValue_Stringify(const struct SeaJsonValue* value, struct SeaStringBuffer* buffer) {
	if (!value) return;

	switch (value->type) {
	case SEAJSON_NULL:
		SeaStringBuffer.append(buffer, "null");
		break;
	case SEAJSON_BOOL:
		SeaStringBuffer.append(buffer, value->boolean ? "true" : "false");
		break;
	case SEAJSON_NUMBER: {
		char num_str[32];
		snprintf(num_str, sizeof(num_str), "%.15g", value->number);
		SeaStringBuffer.append(buffer, num_str);
		break;
	}
	case SEAJSON_STRING:
		SeaStringBuffer.append(buffer, "\"");
		SeaStringBuffer.append(buffer, value->string);
		SeaStringBuffer.append(buffer, "\"");
		break;
	case SEAJSON_ARRAY:
		SeaStringBuffer.append(buffer, "[");
		for (size_t i = 0; i < value->array.count; i++) {
			if (i > 0) SeaStringBuffer.append(buffer, ",");
			JsonValue_Stringify(value->array.items[i], buffer);
		}
		SeaStringBuffer.append(buffer, "]");
		break;
	case SEAJSON_OBJECT:
		SeaStringBuffer.append(buffer, "{");
		for (size_t i = 0; i < value->object.count; i++) {
			if (i > 0) SeaStringBuffer.append(buffer, ",");
			SeaStringBuffer.append(buffer, "\"");
			SeaStringBuffer.append(buffer, value->object.keys[i]);
			SeaStringBuffer.append(buffer, "\":");
			JsonValue_Stringify(value->object.values[i], buffer);
		}
		SeaStringBuffer.append(buffer, "}");
		break;
	}
}

// =========================================
// MARK: JsonValue
// =========================================

static struct SeaJsonValue* JsonValue_parse(const char* string, struct SeaAllocator* allocator) {
	if (!string || !allocator) return NULL;

	InternalJsonParser parser = {
		.json = string,
		.pos = 0,
		.len = strlen(string),
		.alloc = allocator
	};

	return Parse_Value(&parser);
}

static char* JsonValue_toString(const struct SeaJsonValue* self, struct SeaAllocator* alloc) {
	if (!self || !alloc || !alloc->alloc) return NULL;

	struct SeaStringBuffer buffer = {
		.data = (char*)alloc->alloc(alloc->context, 256, alignof(char)),
		.size = 256,
		.pos = 0,
		.allocator = alloc
	};

	if (!buffer.data) return NULL;
	buffer.data[0] = '\0';

	JsonValue_Stringify(self, &buffer);

	return buffer.data;
}

static void JsonValue_free(struct SeaJsonValue* self, struct SeaAllocator* alloc) {
	if (!self || !alloc) return;

	switch (self->type) {
	case SEAJSON_STRING:
		if (alloc->free && self->string) {
			alloc->free(alloc->context, self->string, strlen(self->string) + 1);
		}
		break;
	case SEAJSON_ARRAY:
		for (size_t i = 0; i < self->array.count; i++) {
			JsonValue_free(self->array.items[i], alloc);
		}
		if (alloc->free && self->array.items) {
			alloc->free(alloc->context, self->array.items,
						self->array.capacity * sizeof(struct SeaJsonValue*));
		}
		break;
	case SEAJSON_OBJECT:
		for (size_t i = 0; i < self->object.count; i++) {
			if (alloc->free && self->object.keys[i]) {
				alloc->free(alloc->context, self->object.keys[i], strlen(self->object.keys[i]) + 1);
			}
			JsonValue_free(self->object.values[i], alloc);
		}
		if (alloc->free) {
			if (self->object.keys) {
				alloc->free(alloc->context, self->object.keys, self->object.capacity * sizeof(char*));
			}
			if (self->object.values) {
				alloc->free(alloc->context, self->object.values, self->object.capacity * sizeof(struct SeaJsonValue*));
			}
		}
		break;
	default:
		break;
	}

	if (alloc->free) {
		alloc->free(alloc->context, self, sizeof(struct SeaJsonValue));
	}
}

const struct SeaJsonValue_CLS SeaJsonValue = {
	.parse = JsonValue_parse,
	.toString = JsonValue_toString,
	.free = JsonValue_free,
};

// ===================================
// MARK: JsonObject
// ===================================

void JsonObject_put(struct SeaJsonValue* object, const char* key, struct SeaJsonValue* value, struct SeaAllocator* alloc) {
	if (!object || object->type != SEAJSON_OBJECT || !key || !value || !alloc || !alloc->alloc) return;

	// Check if the key already exists
	for (size_t i = 0; i < object->object.count; i++) {
		if (strcmp(object->object.keys[i], key) == 0) {
			JsonValue_free(object->object.values[i], alloc);
			object->object.values[i] = value;
			return;
		}
	}

	// Add a new key-value pair
	if (object->object.count >= object->object.capacity) {
		const size_t new_capacity = object->object.capacity == 0 ? 4 : object->object.capacity * 2;

		char** new_keys = alloc->alloc(alloc->context, new_capacity * sizeof(char*), sizeof(void*));
		struct SeaJsonValue** new_values = alloc->alloc(alloc->context, new_capacity * sizeof(struct SeaJsonValue*), sizeof(void*));
		if (!new_keys || !new_values) return;

		// Copy old data
		if (object->object.keys) {
			for (size_t i = 0; i < object->object.count; i++) {
				new_keys[i] = object->object.keys[i];
				new_values[i] = object->object.values[i];
			}
			if (alloc->free) {
				alloc->free(alloc->context, object->object.keys,
							object->object.capacity * sizeof(char*));
				alloc->free(alloc->context, object->object.values,
							object->object.capacity * sizeof(struct SeaJsonValue*));
			}
		}

		object->object.keys = new_keys;
		object->object.values = new_values;
		object->object.capacity = new_capacity;
	}

	object->object.keys[object->object.count] = SeaAllocator.strdup(alloc, key);
	object->object.values[object->object.count] = value;
	object->object.count++;
}

struct SeaJsonValue* JsonObject_get(const struct SeaJsonValue* object, const char* key) {
	if (!object || object->type != SEAJSON_OBJECT || !key) return NULL;

	for (size_t i = 0; i < object->object.count; i++) {
		if (strcmp(object->object.keys[i], key) == 0) {
			return object->object.values[i];
		}
	}

	return NULL;
}

const struct SeaJsonObject_CLS SeaJsonObject = {
	.put = JsonObject_put,
	.get = JsonObject_get,
};

// ===================================
// MARK: JsonArray
// ===================================

void JsonArray_add(struct SeaJsonValue* array, struct SeaJsonValue* value, struct SeaAllocator* alloc) {
	if (!array || array->type != SEAJSON_ARRAY || !value || !alloc || !alloc->alloc) return;

	if (array->array.count >= array->array.capacity) {
		const size_t new_capacity = array->array.capacity == 0 ? 4 : array->array.capacity * 2;
		const size_t new_size = new_capacity * sizeof(struct SeaJsonValue*);

		struct SeaJsonValue** new_items = alloc->alloc(alloc->context, new_size, sizeof(void*));
		if (!new_items) return;

		// Copy old items
		if (array->array.items) {
			for (size_t i = 0; i < array->array.count; i++) {
				new_items[i] = array->array.items[i];
			}
			if (alloc->free) {
				alloc->free(alloc->context, array->array.items, array->array.capacity * sizeof(struct SeaJsonValue*));
			}
		}

		array->array.items = new_items;
		array->array.capacity = new_capacity;
	}

	array->array.items[array->array.count++] = value;
}

struct SeaJsonValue* JsonArray_get(const struct SeaJsonValue* array, const size_t index) {
	if (!array || array->type != SEAJSON_ARRAY || index >= array->array.count) return NULL;
	return array->array.items[index];
}

const struct SeaJsonArray_CLS SeaJsonArray = {
	.add = JsonArray_add,
	.get = JsonArray_get,
};
