#ifndef RANDOM_SEALIB_d7a966a2_H
#define RANDOM_SEALIB_d7a966a2_H

#include <stdint.h>
#include <stdbool.h>

struct SeaUUID {
	uint8_t bytes[16];
};

extern const struct SeaUUID_CLS {
	void (* generateV4)(struct SeaUUID* self);
	void (* generateV7)(struct SeaUUID* self);
	void (* toString)(struct SeaUUID* self, char out[37]);
	bool (* equals)(const void* a, const void* b);
} SeaUUID;

#endif// RANDOM_SEALIB_d7a966a2_H