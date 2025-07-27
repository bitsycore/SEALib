#include "Person.h"
#include "Sea/Random.h"
#include "Sea/Arena.h"

#include <stdio.h>
#include <stdalign.h>

#define PRIV(self) ((struct PersonPrivate*) &self->private)

static const char TO_STRING_FORMAT[] = "Person(name: %s, age: %d, uuid: %s)";

static void init(struct Person* self, const char* name, int age) {
	self->name = name;
	self->age = age;
	SeaUUID.generateV4(&PRIV(self)->uuid);
}

static void initWithName(struct Person* self, const char* name) {
	init(self, name, (int) (SeaRandom.randUint64() % 100));
}

static size_t toStringSize(struct Person* self) {
	return snprintf(NULL, 0, TO_STRING_FORMAT, self->name, self->age, "F") + 1 /*NULL Terminated*/ + 35 /*UUID (36) - F (1) from (%s)*/;
}

static char* toString(struct Person* self, struct SeaAllocator* allocator) {
	char uuid[37];
	SeaUUID.toString(&PRIV(self)->uuid, uuid);
	size_t size = toStringSize(self);
	void* buffer = allocator->alloc(allocator->context, sizeof(char) * toStringSize(self), alignof(char));
	snprintf((char*) buffer, size, TO_STRING_FORMAT, self->name, self->age, uuid);
	return (char*) buffer;
}

static bool equals(struct Person* self, struct Person* other) {
	return SeaUUID.equals(&PRIV(self)->uuid, &PRIV(other)->uuid);
}

static struct SeaUUID getId(struct Person* self) {
	return PRIV(self)->uuid;
}

const struct Person_CLS Person = {
	.init = init,
	.initWithName = initWithName,
	.toString = toString,
	.toStringSize = toStringSize,
	.equals = equals,
	.getId = getId,
};