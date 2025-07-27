#include "Person.h"

#include <Sea/Allocator.h>
#include <Sea/Arena.h>
#include <Sea/Random.h>

#include <stdio.h>

#if !defined(_WIN32) && !defined(_WIN64)
#include <alloca.h>
#include <malloc.h>
#else
#include <malloc.h>
#endif

const char* randomName() {
	switch (SeaRandom.randUint64() % 16) {
		case 0: return "Bob";
		case 1: return "Claude";
		case 2: return "Alice";
		case 3: return "John";
		case 4: return "Leon";
		case 5: return "Mary";
		case 6: return "Jane";
		case 7: return "Paulo";
		case 8: return "Jose";
		case 9: return "Maria";
		case 10: return "Ana";
		case 11: return "Edouard";
		case 12: return "Rafael";
		case 13: return "Marc";
		case 14: return "Josiane";
		default: return "Martin";
	}
}

void arena_test(struct SeaArena* arena) {

	struct SeaAllocator arena_allocator = SeaArena.getAllocator(arena);

	struct Person* me = SeaArena.alloc(arena, sizeof(struct Person));
	Person.initWithName(me, randomName());

	struct Person* you = &(struct Person){};
	Person.init(you, randomName(), (int) (SeaRandom.randUint64() % 100));

	struct Person* maybeMe = malloc(sizeof(struct Person));
	*maybeMe = *me;

	char* meStr = Person.toString(me, SeaAllocator.malloc);
	puts(meStr);
	free(meStr);

	char* youStr = Person.toString(you, &arena_allocator);
	puts(youStr);

	if (Person.equals(me, you)) {
		puts("They are equal!");
	} else {
		puts("They are not equal!");
	}

	if (Person.equals(me, maybeMe)) {
		puts("They are equal!");
	} else {
		puts("They are not equal!");
	}

	printf("Remaining: %zu\n", SeaArena.remaining(arena));

	free(maybeMe);
}

int main() {
	size_t arenaSize = 128;
	uint8_t* arenaAndBuffer = malloc(arenaSize + sizeof(SeaArena));
	struct SeaArena* arena = (struct SeaArena*) arenaAndBuffer;
	SeaArena.init(arena, arenaAndBuffer + sizeof(struct SeaArena), arenaSize);

	for (int i = 0; i < 100; i++) {
		arena_test(arena);
		SeaArena.reset(arena);
		puts("-----------------------------");
	}

	free(arenaAndBuffer);
	return 0;
}