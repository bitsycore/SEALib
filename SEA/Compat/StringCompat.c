#include "StringCompat.h"

#if defined(__STDC_LIB_EXT1__) || (defined(_MSC_VER) && _MSC_VER >= 1400) || defined(__MINGW32__)
#else

#include <errno.h>

int SEA_strncpy_s(char *restrict dest, const size_t destsz, const char *restrict src, const size_t count) {
    if (!dest || !src || destsz == 0) return EINVAL;
    if (count > destsz) {
        if (destsz > 0) dest[0] = '\0';
        return ERANGE;
    }

    size_t i = 0;
    while (i < count && src[i] != '\0') {
        dest[i] = src[i];
        ++i;
    }
    dest[i] = '\0';

    return 0;
}

int SEA_strcpy_s(char* restrict dest, const size_t destsz, const char* restrict src) {
    if (!dest || !src || destsz == 0) return EINVAL;
    const size_t src_len = strlen(src);
    if (src_len >= destsz) {
        if (destsz > 0) {
            dest[0] = '\0';
        }
        return ERANGE;
    }
    memcpy(dest, src, src_len + 1);
    return 0;
}

#endif