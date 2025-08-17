#ifndef SEALIB_BITWISE_H
#define SEALIB_BITWISE_H

#include <stdint.h>

int SEA_count_leading_zeros(uint64_t x);
#define SEA_log2i(X) ((uint32_t) (8*sizeof(uint64_t) - SEA_count_leading_zeros((X)) - 1))

#endif //SEALIB_BITWISE_H