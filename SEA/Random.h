#ifndef RANDOM_SEALIB_cfb07073_H
#define RANDOM_SEALIB_cfb07073_H

#include "Config/CompConfig.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint64_t SEA_Random_Uint64();
uint32_t SEA_Random_Uint32();
int64_t SEA_Random_Int64();
int32_t SEA_Random_Int32();
uint16_t SEA_Random_Uint16();
int16_t SEA_Random_Int16();
uint8_t SEA_Random_Uint8();
int8_t SEA_Random_Int8();
float SEA_Random_Float();
double SEA_Random_Double();
bool SEA_Random_Bool();
void SEA_Random_ByteArray(uint8_t* buffer, size_t size);

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

// =========================
// Static
static inline uint64_t Random_Uint64(void) { return SEA_Random_Uint64(); }
static inline uint32_t Random_Uint32(void) { return SEA_Random_Uint32(); }
static inline int64_t Random_Int64(void) { return SEA_Random_Int64(); }
static inline int32_t Random_Int32(void) { return SEA_Random_Int32(); }
static inline uint16_t Random_Uint16(void) { return SEA_Random_Uint16(); }
static inline int16_t Random_Int16(void) { return SEA_Random_Int16(); }
static inline uint8_t Random_Uint8(void) { return SEA_Random_Uint8(); }
static inline int8_t Random_Int8(void) { return SEA_Random_Int8(); }
static inline float Random_Float(void) { return SEA_Random_Float(); }
static inline double Random_Double(void) { return SEA_Random_Double(); }
static inline bool Random_Bool(void) { return SEA_Random_Bool(); }
static inline void Random_ByteArray(uint8_t* buffer, const size_t size) { SEA_Random_ByteArray(buffer, size); }

#endif

#endif //RANDOM_SEALIB_cfb07073_H
