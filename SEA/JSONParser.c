#include "JSONParser.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "Allocator.h"
#include "Error.h"
#include "JSONArray.h"
#include "JSONObject.h"
#include "JSONValue.h"
#include "Compat/StringCompat.h"

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

	const char* start_ptr = parser->json + parser->pos;
	const char* p = start_ptr;

	// First, find the closing quote to identify the full escaped string slice.
	while (p < parser->json + parser->len && *p != '"') {
		if (*p == '\\' && p + 1 < parser->json + parser->len) {
			p += 2;
		} else {
			p++;
		}
	}

	if (p >= parser->json + parser->len) return NULL; // Unterminated string

	const size_t escaped_len = p - start_ptr;
	char* unescaped_str = SEA_Allocator.alloc(parser->allocator, escaped_len + 1);
	if (!unescaped_str) return NULL;

	char* dest = unescaped_str;
	p = start_ptr; // Reset p to the beginning of the string content.

	// Copy and unescape the string content.
	while (p < start_ptr + escaped_len) {
		if (*p == '\\' && p + 1 < start_ptr + escaped_len + 1) {
			p++; // Move to the character after '\'
			switch (*p) {
				case '"':  *dest++ = '"'; break;
				case '\\': *dest++ = '\\'; break;
				case '/':  *dest++ = '/'; break;
				case 'b':  *dest++ = '\b'; break;
				case 'f':  *dest++ = '\f'; break;
				case 'n':  *dest++ = '\n'; break;
				case 'r':  *dest++ = '\r'; break;
				case 't':  *dest++ = '\t'; break;
				// TODO: Full JSON compliance need handle \uXXXX sequences.
				// Copy unknown escape sequences as-is.
				default:   *dest++ = *p; break;
			}
			p++;
		} else {
			*dest++ = *p++;
		}
	}

	// Null-terminate the destination string.
	*dest = '\0';

	// Move the parser position past the closing quote.
	parser->pos += escaped_len + 1;

	struct SEA_JSONValue* value = SEA_JSONValue.CreateString(unescaped_str, parser->allocator);
	SEA_Allocator.free(parser->allocator, unescaped_str);

	return value;
}


static struct SEA_JSONValue* ParseNumber(struct SEA_JSONParser* parser) {
	const char* start_ptr = parser->json + parser->pos;
	char* end_ptr;

	const double num = strtod(start_ptr, &end_ptr);

	if (end_ptr == start_ptr) {
		return NULL;
	}

	parser->pos = end_ptr - parser->json;

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

// =======================================
// MARK: Public
// =======================================

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
