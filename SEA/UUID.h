#ifndef RANDOM_SEALIB_d7a966a2_H
#define RANDOM_SEALIB_d7a966a2_H

#include <stdbool.h>
#include <stdint.h>

struct SEA_UUID {
	uint8_t bytes[16];
};

extern const struct SEA_UUID_CLS {
	void (* generateV4)(struct SEA_UUID* self);
	void (* generateV7)(struct SEA_UUID* self);
	void (* toString)(const struct SEA_UUID* self, char out[37]);
	bool (* equals)(const struct SEA_UUID* a, const struct SEA_UUID* b);
} SEA_UUID;

#endif// RANDOM_SEALIB_d7a966a2_H