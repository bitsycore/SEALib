#include "Person.h"

#include "Sea/Arena.h"
#include "Sea/Random.h"

#include <stdio.h>

#include "Sea/Compat.h"

#define PRIV(self) ((struct PersonPrivate*) &self->private)

static const char TO_STRING_FORMAT[] = "Person(name: %s, age: %d, uuid: %s)";

static void init(struct Person* self, const char* name, const int age) {
	self->name = name;
	self->age = age;
	memset(self->private, 0, sizeof(self->private));
	SeaUUID.generateV7(&PRIV(self)->uuid);
}

static void initWithName(struct Person* self, const char* name) {
	init(self, name, (int) (SeaRandom.RandUint64() % 100));
}

static size_t toStringSize(const struct Person* self) {
	return snprintf(NULL, 0, TO_STRING_FORMAT, self->name, self->age, "F") + 1 /*NULL Terminated*/ + 35 /*UUID (36) - F (1) from (%s)*/;
}

static char* toString(struct Person* self, const struct SeaAllocator* allocator) {
	char uuid[37];
	SeaUUID.toString(&PRIV(self)->uuid, uuid);
	const size_t size = toStringSize(self);
	void* buffer = allocator->alloc(allocator->context, sizeof(char) * toStringSize(self));
	snprintf((char*) buffer, size, TO_STRING_FORMAT, self->name, self->age, uuid);
	return buffer;
}

static bool equals(struct Person* self, struct Person* other) {
	if (self == other) return true;
	return SeaUUID.equals(&PRIV(self)->uuid, &PRIV(other)->uuid);
}

static struct SeaUUID getUUID(struct Person* self) {
	return PRIV(self)->uuid;
}

const struct Person_CLS Person = {
	.init = init,
	.initWithName = initWithName,
	.toString = toString,
	.toStringSize = toStringSize,
	.equals = equals,
	.getUUID = getUUID,
};