#ifndef TIME_SEA_H
#define TIME_SEA_H

#include "Config/CompConfig.h"

#include <stdint.h>

// =======================================
// MARK: Static
// =======================================

uint64_t SEA_Time_GetMillis();

// =======================================
// MARK: Alias
// =======================================

#if SEA_CONFIG_ENABLE_PREFIXLESS == 1

// =========================
// Static
static inline uint64_t Time_GetMillis() { return SEA_Time_GetMillis(); }

#endif

#endif// TIME_SEA_H