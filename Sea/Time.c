#include "Time.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#else
	#include <memory.h>
	#include <time.h>
	#include <unistd.h>
#endif

static uint64_t getMillis() {
#if defined(_POSIX_VERSION)
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#elif defined(_WIN32) || defined(_WIN64)
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	uint64_t time = ((uint64_t) ft.dwHighDateTime << 32) | ft.dwLowDateTime;
	return (time - 116444736000000000ULL) / 10000;
#else
	const time_t seconds = time(NULL);
	return (uint64_t)seconds * 1000;
#endif
}

const struct SeaTime_CLS SeaTime = {
	.getMillis = getMillis
};