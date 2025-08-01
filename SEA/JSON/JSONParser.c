#include "JSONParser.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "JSONArray.h"
#include "JSONObject.h"
#include "JSONValue.h"

#include "../Allocator.h"
#include "../Error.h"

// ===================================
// MARK: Utility
// ===================================

static void SkipWhitespace(SEA_JSONParser* parser) {
	while (parser->pos < parser->len && isspace(parser->json[parser->pos])) {
		parser->pos++;
	}
}

// ===================================
// MARK: Parse
// ===================================

static struct SEA_JSONValue* ParseValue(SEA_JSONParser* parser);

static bool ParseLiteral(SEA_JSONParser* parser, const char* literal) {
	const size_t len = strlen(literal);
	if (parser->pos + len > parser->len) return false;
	if (strncmp(parser->json + parser->pos, literal, len) != 0) return false;
	parser->pos += len;
	return true;
}

static struct SEA_JSONValue* ParseString(SEA_JSONParser* parser) {
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
	char* str = SEA_Allocator.alloc(parser->allocator, len + 1);

	if (!str) return NULL;
	strncpy(str, parser->json + start, len);
	str[len] = '\0';
	parser->pos++; // Skip closing quote

	struct SEA_JSONValue* value = SEA_JSONValue.CreateString(str, parser->allocator);
	SEA_Allocator.free(parser->allocator, str);

	return value;
}

static struct SEA_JSONValue* ParseNumber(SEA_JSONParser* parser) {
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
	char* num_str = SEA_Allocator.alloc(parser->allocator, len + 1);
	if (!num_str) return NULL;
	strncpy(num_str, parser->json + start, len);
	num_str[len] = '\0';

	const double num = atof(num_str);
	SEA_Allocator.free(parser->allocator, num_str);

	return SEA_JSONValue.CreateNumber(num, parser->allocator);
}

static struct SEA_JSONValue* ParseArray(SEA_JSONParser* parser) {
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

static struct SEA_JSONValue* ParseObject(SEA_JSONParser* parser) {
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

static struct SEA_JSONValue* ParseValue(SEA_JSONParser* parser) {
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
	SEA_JSONParser parser = {
		.json = string,
		.pos = 0,
		.len = len == 0 ? strlen(string) : len,
		.allocator = allocator
	};
	return ParseValue(&parser);
}
