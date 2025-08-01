#include "Error.h"

#include "Compat.h"

static SEA_THREAD_LOCAL enum SEA_ErrorType THREAD_ERRNO = 0;

void SetErrno(const enum SEA_ErrorType error) {
	THREAD_ERRNO = error;
}

int GetErrno() {
	return THREAD_ERRNO;
}

const struct SEA_Error_CLS SEA_Error = {
	.SetError = SetErrno,
	.GetError = GetErrno,
};