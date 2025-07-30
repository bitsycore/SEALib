#ifndef RANDOM_SEALIB_cfb07073_H
#define RANDOM_SEALIB_cfb07073_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern const struct SeaRandom_CLS {
	uint64_t (* RandUint64)();
	uint32_t (* RandUint32)();
	int64_t (* RandInt64)();
	int32_t (* RandInt32)();
	uint16_t (* RandUint16)();
	int16_t (* RandInt16)();
	uint8_t (* RandUint8)();
	int8_t (* RandInt8)();
	float (* RandFloat)();
	double (* RandDouble)();
	bool (* RandBool)();
	void (* FillRandomBytes)(uint8_t* buffer, size_t size);
} SeaRandom;

#endif //RANDOM_SEALIB_cfb07073_H