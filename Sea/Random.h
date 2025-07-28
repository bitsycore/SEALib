#ifndef RANDOM_SEALIB_cfb07073_H
#define RANDOM_SEALIB_cfb07073_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern const struct SeaRandom_CLS {
	uint64_t (* randUint64)();
	uint32_t (* randUint32)();
	int64_t (* randInt64)();
	int32_t (* randInt32)();
	uint16_t (* randUint16)();
	int16_t (* randInt16)();
	uint8_t (* randUint8)();
	int8_t (* randInt8)();
	float (* randFloat)();
	double (* randDouble)();
	bool (* randBool)();
	void (* fillRandomBytes)(uint8_t* buffer, size_t size);
} SeaRandom;

#endif //RANDOM_SEALIB_cfb07073_H