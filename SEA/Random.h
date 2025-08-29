#ifndef RANDOM_SEALIB_cfb07073_H
#define RANDOM_SEALIB_cfb07073_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// =======================================
// MARK: Static
// =======================================

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

#endif //RANDOM_SEALIB_cfb07073_H
