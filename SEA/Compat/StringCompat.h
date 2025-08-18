#ifndef SEA_STRINGCOMPAT_H
#define SEA_STRINGCOMPAT_H

#include <string.h>

#if defined(__STDC_LIB_EXT1__) || (defined(_MSC_VER) && _MSC_VER >= 1400) || defined(__MINGW32__)

#define SEA_strcpy_s(dest_, destsize_, src_) strncpy_s((dest_), (destsize_), (src_))
#define SEA_strncpy_s(dest_, size_, src_, count_) strncpy_s((dest_), (size_), (src_), (count_))

#else

int SEA_strcpy_s(char* restrict dest, size_t destsz, const char* restrict src);
int SEA_strncpy_s(char *restrict dest, size_t destsz, const char *restrict src, size_t count);

#endif

#endif//SEA_STRINGCOMPAT_H