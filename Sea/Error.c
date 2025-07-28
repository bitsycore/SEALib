#include "Error.h"

#include "ThreadLocal.h"

SEA_THREAD_LOCAL int LOCAL_ERRNO = 0;

void setErrno(const int errno) {
	LOCAL_ERRNO = errno;
}

int getErrno() {
	return LOCAL_ERRNO;
}

const struct SeaError_CLS SeaError = {
	.setError = setErrno,
	.getError = getErrno,
};