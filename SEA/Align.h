#ifndef SEA_ALIGN_H
#define SEA_ALIGN_H

// =================================
// MARK: Max Align
// =================================

typedef union {
  char c;
  short s;
  int i;
  long l;
  long long ll;
  float f;
  double d;
  long double ld;
  void* p;
} SEA_MaxAlign;

// =================================
// MARK: Alignof
// =================================

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
  #include <stdalign.h>
  #define SEA_ALIGNOF(type) alignof(type)
#elif defined(__GNUC__) || defined(__clang__)
  #define SEA_ALIGNOF(type) __alignof__(type)
#else
  #include <stddef.h>
  #define SEA_ALIGNOF(type) offsetof(struct { char c; type x; }, x)
#endif

#endif //SEA_ALIGN_H