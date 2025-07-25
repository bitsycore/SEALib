// ============================================
// File: Random.sea
// ============================================

#ifndef RANDOM_SEA_cfb07073_H
#define RANDOM_SEA_cfb07073_H

#include <stdint.h>
#include <stdbool.h>

extern const struct Random_CLS {
    /* PUBLIC STATIC METHODS */
    uint64_t (*randUint64)();
    void (*randUuidV4)(char out[37]);
} Random;

#endif //RANDOM_SEA_cfb07073_H