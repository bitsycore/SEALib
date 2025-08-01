#include "JSONParser.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "Allocator.h"
#include "Error.h"
#include "JSONArray.h"
#include "JSONObject.h"
#include "JSONValue.h"
#include "SEA/String.h"

// ===================================
// MARK: Utility
// ===================================

static void SkipWhitespace(struct SEA_JSONParser* parser) {
	while (parser->pos < parser->len && isspace(parser->json[parser->pos])) {
		parser->pos++;
	}
}

// ===================================
// MARK: Parse
// ===================================

static struct SEA_JSONValue* ParseValue(struct SEA_JSONParser* parser);

static bool ParseLiteral(struct SEA_JSONParser* parser, const char* literal) {
	const size_t len = strlen(literal);
	if (parser->pos + len > parser->len) return false;
	if (strncmp(parser->json + parser->pos, literal, len) != 0) return false;
	parser->pos += len;
	return true;
}

static struct SEA_JSONValue* ParseString(struct SEA_JSONParser* parser) {
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

	const size_t str_len = parser->pos - start;
	const size_t alloc_len = str_len + 1;
	char* str = SEA_Allocator.alloc(parser->allocator, alloc_len);
	if (!str) return NULL;
	SEA_strncpy_s(str, alloc_len, parser->json + start, str_len);

	parser->pos++; // Skip closing quote

	struct SEA_JSONValue* value = SEA_JSONValue.CreateString(str, parser->allocator);
	SEA_Allocator.free(parser->allocator, str);

	return value;
}

static struct SEA_JSONValue* ParseNumber(struct SEA_JSONParser* parser) {
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
	const size_t alloc_len = len + 1;
	char* num_str = SEA_Allocator.alloc(parser->allocator, alloc_len);
	if (!num_str) return NULL;
	SEA_strncpy_s(num_str, alloc_len, parser->json + start, len);

	const double num = atof(num_str);
	SEA_Allocator.free(parser->allocator, num_str);

	return SEA_JSONValue.CreateNumber(num, parser->allocator);
}

static struct SEA_JSONValue* ParseArray(struct SEA_JSONParser* parser) {
	if (parser->pos >= parser->len || parser->json[parser->pos] != '[') return NULL;
	parser->pos++; // Skip '['

	struct SEA_JSONValue* array = SEA_JSONArray.New(parser->allocator);
	if (!array) return NULL;

	SkipWhitespace(parser);

	// Empty array case
	if (parser->pos < parser->len && parser->json[parser->pos] == ']') {
		parser->pos++;
		return array;
	}

	while (parser->pos < parser->len) {
		struct SEA_JSONValue* value = ParseValue(parser);
		if (!value) {
			SEA_JSONValue.free(array, parser->allocator);
			return NULL;
		}

		SEA_JSONArray.add(array->array, value, parser->allocator);

		SkipWhitespace(parser);

		if (parser->pos >= parser->len) break;

		if (parser->json[parser->pos] == ']') {
			parser->pos++;
			return array;
		}

		if (parser->json[parser->pos] != ',') {
			SEA_JSONValue.free(array, parser->allocator);
			return NULL;
		}

		parser->pos++; // Skip ','
		SkipWhitespace(parser);
	}

	SEA_JSONValue.free(array, parser->allocator);
	return NULL;
}

static struct SEA_JSONValue* ParseObject(struct SEA_JSONParser* parser) {
	if (parser->pos >= parser->len || parser->json[parser->pos] != '{') return NULL;
	parser->pos++; // Skip '{'

	struct SEA_JSONValue* object = SEA_JSONObject.New(parser->allocator);
	if (!object) return NULL;

	SkipWhitespace(parser);

	if (parser->pos < parser->len && parser->json[parser->pos] == '}') {
		parser->pos++;
		return object;
	}

	while (parser->pos < parser->len) {
		SkipWhitespace(parser);

		struct SEA_JSONValue* key = ParseString(parser);
		if (!key) {
			SEA_JSONValue.free(object, parser->allocator);
			return NULL;
		}

		SkipWhitespace(parser);

		if (parser->pos >= parser->len || parser->json[parser->pos] != ':') {
			SEA_JSONValue.free(key, parser->allocator);
			SEA_JSONValue.free(object, parser->allocator);
			return NULL;
		}

		parser->pos++; // Skip ':'
		SkipWhitespace(parser);

		struct SEA_JSONValue* value = ParseValue(parser);
		if (!value) {
			SEA_JSONValue.free(key, parser->allocator);
			SEA_JSONValue.free(object, parser->allocator);
			return NULL;
		}

		const enum SEA_ErrorType error = SEA_JSONObject.put(object->object, key->string, value, parser->allocator);
		if (error != SEA_ERROR_NONE) {
			return NULL;
		}
		SEA_JSONValue.free(key, parser->allocator);

		SkipWhitespace(parser);

		if (parser->pos >= parser->len) break;

		if (parser->json[parser->pos] == '}') {
			parser->pos++;
			return object;
		}

		if (parser->json[parser->pos] != ',') {
			SEA_JSONValue.free(object, parser->allocator);
			return NULL;
		}

		parser->pos++; // Skip ','
	}

	SEA_JSONValue.free(object, parser->allocator);
	return NULL;
}

static struct SEA_JSONValue* ParseValue(struct SEA_JSONParser* parser) {
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
			return SEA_JSONValue.CreateBool(true);
		}
		return NULL;
	case 'f':
		if (ParseLiteral(parser, "false")) {
			return SEA_JSONValue.CreateBool(false);
		}
		return NULL;
	case 'n':
		if (ParseLiteral(parser, "null")) {
			return SEA_JSONValue.CreateNull();
		}
		return NULL;
	default:
		if (c == '-' || isdigit(c)) {
			return ParseNumber(parser);
		}
		return NULL;
	}
}

struct SEA_JSONValue* JSONParser_FromString(const char* string, const size_t len, struct SEA_Allocator* allocator) {
	if (!string) return NULL;
	if (!allocator || !allocator->alloc) allocator = SEA_Allocator.Malloc;
	struct SEA_JSONParser parser = {
		.json = string,
		.pos = 0,
		.len = len == 0 ? strlen(string) : len,
		.allocator = allocator
	};
	return ParseValue(&parser);
}

const struct SEA_JSONParser_CLS SEA_JSONParser = {
	.FromString = JSONParser_FromString
};
