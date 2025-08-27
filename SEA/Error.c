#include "Error.h"

#include "Compat/ThreadCompat.h"

static SEA_THREAD_LOCAL enum SEA_ErrorType THREAD_ERRNO = 0;

void SEA_Error_SetError(const enum SEA_ErrorType error) {
	THREAD_ERRNO = error;
}

enum SEA_ErrorType SEA_Error_GetError() {
	return THREAD_ERRNO;
}