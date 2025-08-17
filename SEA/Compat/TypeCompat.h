#ifndef SEALIB_TYPECOMPAT_H
#define SEALIB_TYPECOMPAT_H

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
	// C23 standard
	#define SEA_TYPEOF(x) typeof(x)
#elif defined(__GNUC__) || defined(__clang__)
	// GCC or Clang support __typeof__
	#define SEA_TYPEOF(x) __typeof__(x)
#elif defined(_MSC_VER)
	// MSVC supports __typeof__ in some modes (undocumented but works)
	#define SEA_TYPEOF(x) __typeof__(x)
#else
	#error "No compatible typeof/typeof/__typeof__ implementation available for this compiler"
#endif

#endif //SEALIB_TYPECOMPAT_H