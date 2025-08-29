#ifndef SEA_UUID_H
#define SEA_UUID_H

#include <stdbool.h>
#include <stdint.h>

// =======================================
// MARK: Types
// =======================================

typedef struct SEA_UUID {
	uint8_t bytes[16];
} SEA_UUID;

// =======================================
// MARK: Instance
// =======================================

void SEA_UUID_generateV4(SEA_UUID* self);
void SEA_UUID_generateV7(SEA_UUID* self);
bool SEA_UUID_equals(const SEA_UUID* self, const SEA_UUID* other);
void SEA_UUID_toString(const SEA_UUID* self, char out[37]);

#endif //SEA_UUID_H