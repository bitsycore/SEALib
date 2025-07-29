#include "Error.h"

#include "Compat.h"

static SEA_THREAD_LOCAL int THREAD_ERRNO = 0;

void setErrno(const int error) {
	THREAD_ERRNO = error;
}

int getErrno() {
	return THREAD_ERRNO;
}

const struct SeaError_CLS SeaError = {
	.setError = setErrno,
	.getError = getErrno,
};