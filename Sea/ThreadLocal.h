#ifndef THREADLOCAL_SEALIB_3676198b_H
#define THREADLOCAL_SEALIB_3676198b_H

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

#endif// THREADLOCAL_SEALIB_3676198b_H