#include "Uuid.h"
#include "Random.h"

#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>

static uint64_t getUnixMillis() {
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	uint64_t time = ((uint64_t) ft.dwHighDateTime << 32) | ft.dwLowDateTime;
	return (time - 116444736000000000ULL) / 10000;
}

#else

#include <time.h>
#include <memory.h>

static uint64_t getUnixMillis() {
#if defined(_POSIX_VERSION)
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#else
	// Fallback for non-POSIX systems
	time_t seconds = time(NULL);
	return (uint64_t)seconds * 1000;
#endif
}

#endif

static void generateV4(struct Uuid* self) {
	Random.fillRandomBytes(self->bytes, 16);
	// Set version (4) and variant (RFC4122)
	self->bytes[6] = (self->bytes[6] & 0x0F) | 0x40; // Version 4
	self->bytes[8] = (self->bytes[8] & 0x3F) | 0x80; // Variant 10xx
}

static void generateV7(struct Uuid* self) {
	uint64_t now = getUnixMillis();

	// Set timestamp (48 bits, big endian)
	for (int i = 0; i < 6; ++i) {
		self->bytes[i] = (now >> ((5 - i) * 8)) & 0xFF;
	}

	// Set version (7)
	self->bytes[6] = (self->bytes[6] & 0x0F) | 0x70;

	// Fill the remaining 10 bytes with random
	for (int i = 6; i < 16; i += 8) {
		uint64_t r = Random.randUint64();
		for (int j = 0; j < 8 && (i + j) < 16; j++) {
			self->bytes[i + j] = (i + j == 6) ? self->bytes[6] : ((r >> (j * 8)) & 0xFF);
		}
	}

	// Set variant (RFC4122)
	self->bytes[8] = (self->bytes[8] & 0x3F) | 0x80;
}

static bool equals(const void* a, const void* b) {
	return memcmp(a, b, sizeof(struct Uuid)) == 0;
}

static void toString(struct Uuid* self, char out[37]) {
	snprintf(
		out,
		37,
		"%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		self->bytes[0], self->bytes[1], self->bytes[2], self->bytes[3],
		self->bytes[4], self->bytes[5], self->bytes[6], self->bytes[7],
		self->bytes[8], self->bytes[9], self->bytes[10], self->bytes[11],
		self->bytes[12], self->bytes[13], self->bytes[14], self->bytes[15]
	);
}

const struct Uuid_CLS Uuid = {
	.toString = toString,
	.generateV4 = generateV4,
	.generateV7 = generateV7,
	.equals = equals,
};