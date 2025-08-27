#ifndef SEA_UUID_H
#define SEA_UUID_H

#include "Config/CompConfig.h"

#include <stdbool.h>
#include <stdint.h>

struct SEA_UUID {
	uint8_t bytes[16];
};

void SEA_UUID_generateV4(struct SEA_UUID* self);
void SEA_UUID_generateV7(struct SEA_UUID* self);
bool SEA_UUID_equals(const struct SEA_UUID* a, const struct SEA_UUID* b);
void SEA_UUID_toString(const struct SEA_UUID* self, char out[37]);

#endif //SEA_UUID_H