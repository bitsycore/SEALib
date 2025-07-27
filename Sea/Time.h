#ifndef TIME_SEA_H
#define TIME_SEA_H

#include <stdint.h>

extern const struct SeaTime_CLS {
	uint64_t (*getMillis)();
} SeaTime;

#endif// TIME_SEA_H