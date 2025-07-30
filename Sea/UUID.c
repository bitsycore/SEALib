#include "UUID.h"

#include "Random.h"
#include "Time.h"

#include <stdio.h>
#include <string.h>

static void generateV4(struct SeaUUID* self) {
	SeaRandom.FillRandomBytes(self->bytes, 16);
	// Set version (4) and variant (RFC4122)
	self->bytes[6] = (self->bytes[6] & 0x0F) | 0x40; // Version 4
	self->bytes[8] = (self->bytes[8] & 0x3F) | 0x80; // Variant 10xx
}

static void generateV7(struct SeaUUID* self) {
	const uint64_t now = SeaTime.getMillis();

	// Set timestamp (48 bits, big endian)
	for (int i = 0; i < 6; ++i) {
		self->bytes[i] = (now >> ((5 - i) * 8)) & 0xFF;
	}

	// Set version (7)
	self->bytes[6] = (self->bytes[6] & 0x0F) | 0x70;

	// Fill the Sea_remaining 10 bytes with random
	for (int i = 6; i < 16; i += 8) {
		const uint64_t r = SeaRandom.RandUint64();
		for (int j = 0; j < 8 && (i + j) < 16; j++) {
			self->bytes[i + j] = (i + j == 6) ? self->bytes[6] : ((r >> (j * 8)) & 0xFF);
		}
	}

	// Set variant (RFC4122)
	self->bytes[8] = (self->bytes[8] & 0x3F) | 0x80;
}

static bool equals(const struct SeaUUID* a, const struct SeaUUID* b) {
	if (a == b) return true;
	if (a == NULL || b == NULL) return false;
	return memcmp(a->bytes, b->bytes, sizeof(a->bytes)) == 0;
}

static void toString(const struct SeaUUID* self, char out[37]) {
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

const struct SeaUUID_CLS SeaUUID = {
	.toString = toString,
	.generateV4 = generateV4,
	.generateV7 = generateV7,
	.equals = equals,
};
