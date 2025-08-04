#ifndef SEA_STRING_H
#define SEA_STRING_H

#include <string.h>

#if defined(__STDC_LIB_EXT1__) || (defined(_MSC_VER) && _MSC_VER >= 1400) || defined(__MINGW32__)
#define SEA_strncpy_s(dest_, size_, src_, count_) strncpy_s((dest_), (size_), (src_), (count_))
#else
int SEA_strncpy_s(char *restrict dest, const size_t destsz, const char *restrict src, const size_t count);
#endif

#endif // SEA_STRING_H