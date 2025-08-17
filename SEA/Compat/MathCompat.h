#ifndef SEALIB_BITWISE_H
#define SEALIB_BITWISE_H

#include <stdint.h>

int SEA_count_leading_zeros(uint64_t x);
#define SEA_log2i(X) ((uint32_t) (8*sizeof(uint64_t) - SEA_count_leading_zeros((X)) - 1))

#define SEA_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SEA_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SEA_CLAMP(x, min, max) SEA_MAX(min, SEA_MIN(x, max))

#endif //SEALIB_BITWISE_H