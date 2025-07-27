#ifndef SEALIB_ERROR_H
#define SEALIB_ERROR_H

extern const struct SeaError_CLS {
	void (* setError)(int error);
	int (* getError)();
} SeaError;

#endif //SEALIB_ERROR_H
