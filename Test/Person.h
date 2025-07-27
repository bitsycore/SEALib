#ifndef PERSON_3c01bdbb_H
#define PERSON_3c01bdbb_H

#include "Sea/UUID.h"
#include "Sea/Arena.h"

#include <stdint.h>
#include <stdbool.h>

struct PersonPrivate {
	struct SeaUUID uuid;
};

struct Person {
	const char* name;
	int age;
	uint8_t private[sizeof(struct PersonPrivate)];
};

extern const struct Person_CLS {
	void (* init)(struct Person* self, const char* name, int age);
	void (* initWithName)(struct Person* self, const char* name);
	size_t (* toStringSize)(struct Person* self);
	char* (* toString)(struct Person* self, struct SeaAllocator* allocator);
	bool (* equals)(struct Person* self, struct Person* other);
	struct SeaUUID (* getUUID)(struct Person* self);
} Person;

#endif//PERSON_3c01bdbb_H