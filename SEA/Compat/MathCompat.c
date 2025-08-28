#include "MathCompat.h"

#if defined(_MSC_VER)
#include <Windows.h>
#endif

int SEA_count_leading_zeros(const uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
	return __builtin_clzll(x);
#elif defined(_MSC_VER)
	unsigned long index;
	if (_BitScanReverse64(&index, x))
		return 63 - index;
	else
		return 64; // x == 0
#else
	int n = 0;
	if (x == 0) return 64;
	for (unsigned long long mask = 1ULL << 63; !(x & mask); mask >>= 1)
		++n;
	return n;
#endif
}