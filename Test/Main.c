#include "Person.h"

#include <Sea/Allocator.h>
#include <Sea/Arena.h>
#include <Sea/Memory.h>
#include <Sea/Random.h>

#include <stdio.h>
#include <stdlib.h>

#include "Sea/JSON.h"

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
	const struct SeaAllocator arena_allocator = SeaArena.getAllocator(arena);

	struct Person* me = SeaArena.alloc(arena, sizeof(struct Person));
	Person.initWithName(me, randomName());

	struct Person* you = &(struct Person){};
	Person.init(you, randomName(), (int) (SeaRandom.randUint64() % 100));

	struct Person* maybeMe = malloc(sizeof(struct Person));
	*maybeMe = *me;

	char* meStr = Person.toString(me, SeaAllocator.malloc);
	puts(meStr);
	free(meStr);

	const char* youStr = Person.toString(you, &arena_allocator);
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

	// ---- JSON -------------------------------------------------

	const char * str = "{\"menu\": {\n  \"header\": \"SVG Viewer\",\n  \"items\": [\n    {\"id\": \"Open\"},\n    {\"id\": \"OpenNew\", \"label\": \"Open New\"},\n    null,\n    {\"id\": \"ZoomIn\", \"label\": \"Zoom In\"},\n    {\"id\": \"ZoomOut\", \"label\": \"Zoom Out\"},\n    {\"id\": \"OriginalView\", \"label\": \"Original View\"},\n    null,\n    {\"id\": \"Quality\"},\n    {\"id\": \"Pause\"},\n    {\"id\": \"Mute\"},\n    null,\n    {\"id\": \"Find\", \"label\": \"Find...\"},\n    {\"id\": \"FindAgain\", \"label\": \"Find Again\"},\n    {\"id\": \"Copy\"},\n    {\"id\": \"CopyAgain\", \"label\": \"Copy Again\"},\n    {\"id\": \"CopySVG\", \"label\": \"Copy SVG\"},\n    {\"id\": \"ViewSVG\", \"label\": \"View SVG\"},\n    {\"id\": \"ViewSource\", \"label\": \"View Source\"},\n    {\"id\": \"SaveAs\", \"label\": \"Save As\"},\n    null,\n    {\"id\": \"Help\"},\n    {\"id\": \"About\", \"label\": \"About Adobe CVG Viewer...\"}\n  ]\n}}";
	struct SeaJsonValue* abc = SeaJsonValue.parseString(str, SeaAllocator.malloc);
	if (abc->type != SEAJSON_OBJECT) { exit(1); }
	char* abc_str = SeaJsonValue.toString(abc, SeaAllocator.malloc);
	printf("%s\n", abc_str);
	free(abc_str);
	SeaJsonValue.free(abc, SeaAllocator.malloc);

	// ---- Arena -------------------------------------------------

	const size_t arenaSize = 128;
	SEA_MALLOC_SCOPE(arenaSize + sizeof(struct SeaArena)) {
		struct SeaArena* arena = (struct SeaArena*) SCOPE_PTR;
		SeaArena.init(arena, SCOPE_PTR + sizeof(struct SeaArena), arenaSize);
		for (int i = 0; i < 5; i++) {
			SEA_ARENA_SCOPE(arena) {
				arena_test(arena);
				SeaArena.reset(arena);
				puts("-----------------------------");
			}
		}
	}
	return 0;
}