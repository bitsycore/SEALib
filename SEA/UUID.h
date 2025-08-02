#ifndef SEA_UUID_H
#define SEA_UUID_H

#include <stdbool.h>
#include <stdint.h>

struct SEA_UUID {
	uint8_t bytes[16];
};

extern const struct SEA_UUID_CLS {
	void (* generateV4)(struct SEA_UUID* self);
	void (* generateV7)(struct SEA_UUID* self);
	bool (* equals)(const struct SEA_UUID* a, const struct SEA_UUID* b);
	void (* toString)(const struct SEA_UUID* self, char out[37]);
} SEA_UUID;

#endif //SEA_UUID_H