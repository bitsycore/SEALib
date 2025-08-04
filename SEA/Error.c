#include "Error.h"

#include "ThreadCompat.h"

static SEA_THREAD_LOCAL enum SEA_ErrorType THREAD_ERRNO = 0;

void Error_SetError(const enum SEA_ErrorType error) {
	THREAD_ERRNO = error;
}

int Error_GetError() {
	return THREAD_ERRNO;
}

const struct SEA_Error_CLS SEA_Error = {
	.SetError = Error_SetError,
	.GetError = Error_GetError,
};