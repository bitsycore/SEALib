#ifndef PERSON_SEALIB_3c01bdbb_H
#define PERSON_SEALIB_3c01bdbb_H

#include <stdint.h>
#include <stdbool.h>
#include "Uuid.h"
#include "Arena.h"

struct Person {
	const char* name;
	int age;
	struct Uuid id;
};

extern const struct Person_CLS {
	void (* init)(struct Person* self, const char* name, int age);
	void (* initWithName)(struct Person* self, const char* name);
	size_t (* toStringSize)(struct Person* self);
	char* (* toString)(struct Person* self, struct Allocator* allocator);
	bool (* equals)(struct Person* self, struct Person* other);
	struct Uuid (* getId)(struct Person* self);
} Person;

#endif //PERSON_SEALIB_3c01bdbb_H