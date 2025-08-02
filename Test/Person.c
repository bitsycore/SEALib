#include "Person.h"

#include <SEA/Random.h>

#include <stdio.h>
#include <string.h>

static const char TO_STRING_FORMAT[] = "Person(name: %s, age: %d, uuid: %s)";

static void init(struct Person* self, const char* name, const int age) {
	self->name = name;
	self->age = age;
	memset(&self->uuid, 0, sizeof(struct SEA_UUID));
	SEA_UUID.generateV7(&self->uuid);
}

static void initWithName(struct Person* self, const char* name) {
	init(self, name, (int) (SEA_Random.Uint64() % 100));
}

static size_t toStringSize(const struct Person* self) {
	return snprintf(NULL, 0, TO_STRING_FORMAT, self->name, self->age, "F") + 1 /*NULL Terminated*/ + 35 /*UUID (36) - F (1) from (%s)*/;
}

static char* toString(const struct Person* self, const struct SEA_Allocator* allocator) {
	char uuid[37];
	SEA_UUID.toString(&self->uuid, uuid);
	const size_t size = toStringSize(self);
	void* buffer = allocator->alloc(allocator->context, sizeof(char) * size);
	snprintf((char*) buffer, size, TO_STRING_FORMAT, self->name, self->age, uuid);
	return buffer;
}

static bool equals(const struct Person* self, const struct Person* other) {
	if (self == other) return true;
	return SEA_UUID.equals(&self->uuid, &other->uuid);
}

static struct SEA_UUID getUUID(const struct Person* self) {
	return self->uuid;
}

const struct Person_CLS Person = {
	.init = init,
	.initWithName = initWithName,
	.toString = toString,
	.toStringSize = toStringSize,
	.equals = equals,
	.getUUID = getUUID,
};