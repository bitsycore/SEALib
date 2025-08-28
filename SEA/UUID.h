#ifndef SEA_UUID_H
#define SEA_UUID_H

#include "Config/CompConfig.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct SEA_UUID {
	uint8_t bytes[16];
} SEA_UUID;

void SEA_UUID_generateV4(SEA_UUID* self);
void SEA_UUID_generateV7(SEA_UUID* self);
bool SEA_UUID_equals(const SEA_UUID* a, const SEA_UUID* b);
void SEA_UUID_toString(const SEA_UUID* self, char out[37]);

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

// =========================
// Instance
static inline void UUID_generateV4(SEA_UUID* self) { SEA_UUID_generateV4(self); }
static inline void UUID_generateV7(SEA_UUID* self) { SEA_UUID_generateV7(self); }
static inline bool UUID_equals(const SEA_UUID* self, const SEA_UUID* other) { return SEA_UUID_equals(self, other); }
static inline void UUID_toString(const SEA_UUID* self, char out[37]) { SEA_UUID_toString(self, out); }

#endif

#endif //SEA_UUID_H