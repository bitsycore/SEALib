#include "Error.h"

#include "Compat.h"

static SEA_THREAD_LOCAL enum SeaErrorType THREAD_ERRNO = 0;

void setErrno(const enum SeaErrorType error) {
	THREAD_ERRNO = error;
}

int getErrno() {
	return THREAD_ERRNO;
}

const struct SeaError_CLS SeaError = {
	.setError = setErrno,
	.getError = getErrno,
};