#ifndef RANDOM_SEALIB_cfb07073_H
#define RANDOM_SEALIB_cfb07073_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern const struct SEA_Random_CLS {
	uint64_t (* Uint64)();
	uint32_t (* Uint32)();
	int64_t (* Int64)();
	int32_t (* Int32)();
	uint16_t (* Uint16)();
	int16_t (* Int16)();
	uint8_t (* Uint8)();
	int8_t (* Int8)();
	float (* Float)();
	double (* Double)();
	bool (* Bool)();
	void (* Bytes)(uint8_t* buffer, size_t size);
} SEA_Random;

#endif //RANDOM_SEALIB_cfb07073_H