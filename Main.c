#include "Person.h"
#include "Arena.h"
#include "Random.h"
#include "Allocator.h"

#include <stdio.h>

#if !defined(_WIN32) && !defined(_WIN64)
#include <alloca.h>
#include <malloc.h>
#else
#include <malloc.h>
#endif

const char* randomName() {
	switch (Random.randUint64() % 16) {
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

void arena_test(struct Arena* arena) {

	struct Allocator arena_allocator = Arena.getAllocator(arena);
	struct Person* me = Arena.alloc(arena, sizeof(struct Person));

	Person.initWithName(me, randomName());

	struct Person* you = alloca(sizeof(struct Person));
	Person.init(you, randomName(), (int) (Random.randUint64() % 100));

	struct Person* maybeMe = malloc(sizeof(struct Person));
	*maybeMe = *me;

	char* meStr = Person.toString(me, Allocator.malloc);
	puts(meStr);
	//free(meStr);

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

	printf("Remaining: %zu\n", Arena.remaining(arena));

	free(maybeMe);
}

int main() {
	size_t arenaSize = 128;
	uint8_t* arenaAndBuffer = malloc(arenaSize + sizeof(struct Arena));
	struct Arena* arena = (struct Arena*) arenaAndBuffer;
	Arena.init(arena, arenaAndBuffer + sizeof(struct Arena), arenaSize);

	for (int i = 0; i < 100; i++) {
		arena_test(arena);
		Arena.reset(arena);
		puts("-----------------------------");
	}

	free(arenaAndBuffer);
	return 0;
}