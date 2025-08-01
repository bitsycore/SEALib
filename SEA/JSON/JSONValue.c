#include "JSONValue.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "JSONArray.h"
#include "JSONObject.h"
#include "JSONParser.h"
#include "../Allocator.h"
#include "../StringBuffer.h"
#include "../Time.h"

// ===================================
// MARK: Internal
// ===================================

static struct SEA_JSONValue InternalNullSingleton = {
	.type = SEA_JSON_NULL,
	.string = NULL,
	.timestamp = 0
};

static struct SEA_JSONValue InternalTrueSingleton = {
	.type = SEA_JSON_BOOL,
	.boolean = true,
	.timestamp = 0
};

static struct SEA_JSONValue InternalFalseSingleton = {
	.type = SEA_JSON_BOOL,
	.boolean = false,
	.timestamp = 0
};

// ===================================
// MARK: Create
// ===================================

struct SEA_JSONValue* CreateNull() {
	return &InternalNullSingleton;
}

struct SEA_JSONValue* CreateBool(const bool val) {
	return val ? &InternalTrueSingleton : &InternalFalseSingleton;
}

struct SEA_JSONValue* CreateNumber(const double val, struct SEA_Allocator* alloc) {
	if (!alloc || !alloc->alloc) return NULL;

	struct SEA_JSONValue* value = SEA_Allocator.alloc(alloc, sizeof(struct SEA_JSONValue));
	if (!value) return NULL;
	value->type = SEA_JSON_NUMBER;
	value->number = val;
	value->timestamp = SEA_Time.getMillis();

	return value;
}

struct SEA_JSONValue* CreateString(const char* val, struct SEA_Allocator* alloc) {
	if (!alloc || !alloc->alloc || !val) return NULL;

	const size_t bufferLen = sizeof(struct SEA_JSONValue) + strlen(val) + 1;
	uint8_t* buffer =  SEA_Allocator.alloc(alloc, bufferLen);
	if (!buffer) return NULL;
	struct SEA_JSONValue* value = (struct SEA_JSONValue*) buffer;
	value->string = (char*) (buffer + sizeof(struct SEA_JSONValue));
	memcpy(value->string, val, strlen(val) + 1);
	value->type = SEA_JSON_STRING;
	value->timestamp = SEA_Time.getMillis();
	if (!value->string) {
		SEA_Allocator.free(alloc, value);
		return NULL;
	}

	return value;
}

// =========================================
// MARK: Stringify
// =========================================

static size_t MeasureJson(const struct SEA_JSONValue* value) {
	if (!value) return 0;

	size_t size = 0;
	switch (value->type) {
	case SEA_JSON_NULL:
		return 4;
	case SEA_JSON_BOOL:
		return value->boolean ? 4 : 5;
	case SEA_JSON_NUMBER:
		return 25; // generous enough for double
	case SEA_JSON_STRING:
		return strlen(value->string) + 2; // quotes
	case SEA_JSON_ARRAY: {
		size = 2; // []
		for (size_t i = 0; i < value->array->count; ++i) {
			if (i > 0) size += 1; // comma
			size += MeasureJson(value->array->items[i]);
		}
		return size;
	}
	case SEA_JSON_OBJECT: {
		size = 2; // {}
		bool first = true;
		const struct SEA_JSONObject* obj = value->object;
		for (size_t bi = 0; bi < obj->bucketCount; ++bi) {
			const SEA_JSONObjectEntry* e = obj->buckets[bi];
			while (e) {
				if (!first) size += 1; // comma
				first = false;
				size += e->key_len + 3; // "key":
				size += MeasureJson(e->value);
				e = e->next;
			}
		}
		return size;
	}
	default:
		return 0;
	}
}

static void WriteJson(const struct SEA_JSONValue* value, struct SeaStringBuffer* buffer) {
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
		for (size_t i = 0; i < value->array->count; ++i) {
			if (i > 0) SeaStringBuffer.append(buffer, ",");
			WriteJson(value->array->items[i], buffer);
		}
		SeaStringBuffer.append(buffer, "]");
		break;
	case SEA_JSON_OBJECT: {
		SeaStringBuffer.append(buffer, "{");
		bool first = true;
		const struct SEA_JSONObject* obj = value->object;
		for (size_t bi = 0; bi < obj->bucketCount; ++bi) {
			const SEA_JSONObjectEntry* e = obj->buckets[bi];
			while (e) {
				if (!first) SeaStringBuffer.append(buffer, ",");
				first = false;
				SeaStringBuffer.append(buffer, "\"");
				SeaStringBuffer.append(buffer, e->key);
				SeaStringBuffer.append(buffer, "\":");
				WriteJson(e->value, buffer);
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

static struct SEA_JSONValue* JsonValue_FromString(const char* string, const size_t len, struct SEA_Allocator* allocator) {
	return JSONParser_FromString(string, len, allocator);
}

static char* JsonValue_toString(const struct SEA_JSONValue* self, struct SEA_Allocator* allocator) {
	if (!self || !allocator || !allocator->alloc) return NULL;
	struct SeaStringBuffer buffer = {};
	const size_t size = MeasureJson(self);
	SeaStringBuffer.init(&buffer, size, allocator);
	WriteJson(self, &buffer);
	return buffer.data;
}

static void JsonValue_free(struct SEA_JSONValue* self, struct SEA_Allocator* alloc) {
	if (!self) return;
	switch (self->type) {
		case SEA_JSON_ARRAY: SEA_JSONArray.free(self, alloc);
			return;
		case SEA_JSON_OBJECT: SEA_JSONObject.free(self, alloc);
			return;
		default:
			if (self->type != SEA_JSON_NULL && self->type != SEA_JSON_BOOL ) {
				SEA_Allocator.free(alloc, self);
			}
	}
}

const struct SEA_JSONValue_CLS SEA_JSONValue = {
	// Static
	.FromString = JsonValue_FromString,
	.CreateNull = CreateNull,
	.CreateBool = CreateBool,
	.CreateNumber = CreateNumber,
	.CreateString = CreateString,

	// Instance
	.toString = JsonValue_toString,
	.free = JsonValue_free,
};