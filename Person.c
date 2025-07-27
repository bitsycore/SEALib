#include "Person.h"
#include "Random.h"
#include "Arena.h"

#include <stdio.h>
#include <stdalign.h>

static const char* TO_STRING_FORMAT = "Person(name: %s, age: %d, uuid: %s)";

static void init(struct Person* self, const char* name, int age) {
	self->name = name;
	self->age = age;
	Uuid.generateV4(&self->id);
}

static void initWithName(struct Person* self, const char* name) {
	self->name = name;
	self->age = 18;
	Uuid.generateV4(&self->id);
}

static size_t toStringSize(struct Person* self) {
	char uuid[] = "43743841-0b6e-4b89-ae9f-699391fe7a76";
	return snprintf(NULL, 0, TO_STRING_FORMAT, self->name, self->age, uuid) + 1;
}

static char* toString(struct Person* self, struct Allocator* allocator) {
	char uuid[37];
	Uuid.toString(&self->id, uuid);
	size_t size = toStringSize(self);
	void* buffer = allocator->alloc(allocator->context, sizeof(char) * toStringSize(self), alignof(
	char));
	snprintf((char*) buffer, size, TO_STRING_FORMAT, self->name, self->age, uuid);
	return (char*) buffer;
}

static bool equals(struct Person* self, struct Person* other) {
	return Uuid.equals(&self->id, &other->id);
}

static struct Uuid getId(struct Person* self) {
	return self->id;
}

const struct Person_CLS Person = {
	.init = init,
	.initWithName = initWithName,
	.toString = toString,
	.toStringSize = toStringSize,
	.equals = equals,
	.getId = getId,
};