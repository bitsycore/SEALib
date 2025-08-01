#ifndef PERSON_3c01bdbb_H
#define PERSON_3c01bdbb_H

#include <SEA/Arena.h>
#include <SEA/UUID.h>

#include <stdbool.h>

struct Person {
	struct SEA_UUID uuid;
	const char* name;
	int age;
};

extern const struct Person_CLS {
	void (* init)(struct Person* self, const char* name, int age);
	void (* initWithName)(struct Person* self, const char* name);
	size_t (* toStringSize)(const struct Person* self);
	char* (* toString)(const struct Person* self, const struct SEA_Allocator* allocator);
	bool (* equals)(const struct Person* self, const struct Person* other);
	struct SEA_UUID (* getUUID)(const struct Person* self);
} Person;

#endif//PERSON_3c01bdbb_H