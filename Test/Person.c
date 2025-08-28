#include "Person.h"

#include <SEA/Random.h>

#include <stdio.h>
#include <string.h>

static void Person_init(struct Person* self, const char* name, const int age) {
	self->name = name;
	self->age = age;
	memset(&self->uuid, 0, sizeof(SEA_UUID));
	SEA_UUID_generateV7(&self->uuid);
}

static void Person_initWithName(struct Person* self, const char* name) {
	Person_init(self, name, (int) (SEA_Random_Uint64() % 100));
}

static const char TO_STRING_FORMAT[] = "Person(name: %s, age: %d, uuid: %s)";

static size_t Person_toStringSize(const struct Person* self) {
	return snprintf(NULL, 0, TO_STRING_FORMAT, self->name, self->age, "F") + 1 /*NULL Terminated*/ + 35 /*UUID (36) - F (1) from (%s)*/;
}

static char* Person_toString(const struct Person* self, SEA_Allocator* allocator) {
	char uuid[37];
	SEA_UUID_toString(&self->uuid, uuid);
	const size_t size = Person_toStringSize(self);
	void* buffer = allocator->alloc(allocator->context, sizeof(char) * size);
	snprintf((char*) buffer, size, TO_STRING_FORMAT, self->name, self->age, uuid);
	return buffer;
}

static bool Person_equals(const struct Person* self, const struct Person* other) {
	if (self == other) return true;
	return SEA_UUID_equals(&self->uuid, &other->uuid);
}

static SEA_UUID Person_getUUID(const struct Person* self) {
	return self->uuid;
}

const struct Person_CLS Person = {
	.init = Person_init,
	.initWithName = Person_initWithName,
	.toString = Person_toString,
	.toStringSize = Person_toStringSize,
	.equals = Person_equals,
	.getUUID = Person_getUUID,
};