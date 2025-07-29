#ifndef COMPAT_SEALIB_3676198b_H
#define COMPAT_SEALIB_3676198b_H

// =================================
// MARK: Thread-Local
// =================================

#if defined(_MSC_VER)
	#define SEA_THREAD_LOCAL __declspec(thread)
#elif defined(__clang__) || defined(__GNUC__)
	#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && __STDC_VERSION__ < 202311L
		#define SEA_THREAD_LOCAL _Thread_local
	#else
		#define SEA_THREAD_LOCAL __thread
	#endif
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
	#define SEA_THREAD_LOCAL thread_local
#else
	#error "Cannot determine thread-local storage specifier"
#endif

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

#endif// COMPAT_SEALIB_3676198b_H