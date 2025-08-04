#ifndef SEA_MACRO_H
#define SEA_MACRO_H

#define SEA_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SEA_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SEA_CLAMP(x, min, max) SEA_MAX(min, SEA_MIN(x, max))

#endif //SEA_MACRO_H