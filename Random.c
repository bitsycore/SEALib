#include "Random.h"

#include <time.h>

static uint64_t state = 0;

static uint64_t randUint64() {
	if (state == 0) {
		state = (uint64_t) time(NULL) | 1;
	}
	uint64_t x = state;
	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;
	state = x;
	return x * UINT64_C(2685821657736338717);
}

static uint32_t randUint32() {
	return (uint32_t) (randUint64() >> 32);
}

static int64_t randInt64() {
	return (int64_t) randUint64();
}

static int32_t randInt32() {
	return (int32_t) randUint32();
}

static uint16_t randUint16() {
	return (uint16_t) (randUint64() & 0xFFFF);
}

static int16_t randInt16() {
	return (int16_t) (randUint64() & 0xFFFF);
}

static uint8_t randUint8() {
	return (uint8_t) (randUint64() & 0xFF);
}

static int8_t randInt8() {
	return (int8_t) (randUint64() & 0xFF);
}

static float randFloat() {
	return ((float) (randUint64() >> 40)) / 16777216.0f; // 2^24
}

static double randDouble() {
	return ((double) (randUint64() >> 11)) / 9007199254740992.0; // 2^53
}

static bool randBool() {
	return (randUint64() >> 63) != 0;
}

void fillRandomBytes(uint8_t* buffer, size_t length) {
	while (length >= 8) {
		uint64_t r = randUint64();
		for (int i = 0; i < 8; ++i) {
			buffer[i] = (uint8_t) (r & 0xFF);
			r >>= 8;
		}
		buffer += 8;
		length -= 8;
	}

	if (length > 0) {
		uint64_t r = randUint64();
		for (size_t i = 0; i < length; ++i) {
			buffer[i] = (uint8_t) (r & 0xFF);
			r >>= 8;
		}
	}
}

const struct Random_CLS Random = {
	.randUint64 = randUint64,
	.randUint32 = randUint32,
	.randInt64 = randInt64,
	.randInt32 = randInt32,
	.randUint16 = randUint16,
	.randInt16 = randInt16,
	.randUint8 = randUint8,
	.randInt8 = randInt8,
	.randFloat = randFloat,
	.randDouble = randDouble,
	.randBool = randBool,
	.fillRandomBytes = fillRandomBytes,
};