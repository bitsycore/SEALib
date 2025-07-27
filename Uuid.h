#ifndef SEADEMO_UUID_H
#define SEADEMO_UUID_H

#include <stdint.h>
#include <stdbool.h>

struct Uuid {
	uint8_t bytes[16];
};

extern const struct Uuid_CLS {
	void (* generateV4)(struct Uuid* self);
	void (* generateV7)(struct Uuid* self);
	void (* toString)(struct Uuid* self, char out[37]);
	bool (* equals)(const void* a, const void* b);
} Uuid;

#endif //SEADEMO_UUID_H