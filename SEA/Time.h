#ifndef TIME_SEA_H
#define TIME_SEA_H

#include <stdint.h>

extern const struct SEA_Time_CLS {
	uint64_t (*getMillis)();
} SEA_Time;

#endif// TIME_SEA_H