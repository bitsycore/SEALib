#include "Person.h"

#include <Sea/Allocator.h>
#include <Sea/Arena.h>
#include <Sea/Memory.h>
#include <Sea/Random.h>

#include <stdio.h>
#include <stdlib.h>

#include "Sea/JSON.h"

const char* randomName() {
	switch (SeaRandom.RandUint64() % 16) {
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
	Person.init(you, randomName(), (int) (SeaRandom.RandUint64() % 100));

	struct Person* maybeMe = malloc(sizeof(struct Person));
	*maybeMe = *me;

	char* meStr = Person.toString(me, SeaAllocator.Malloc);
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

	puts("");
	puts("=========================================================================");
	puts("*   JSON                                                                *");
	puts("=========================================================================");
	puts("");

	// ---- JSON -------------------------------------------------

	// language=JSON
	const char str[] = "{\n  \"menu\": {\n    \"header\": \"SVG Viewer\",\n    \"items\": [\n      {\n        \"id\": \"Open\"\n      },\n      {\n        \"id\": \"OpenNew\",\n        \"label\": \"Open New\"\n      },\n      null,\n      {\n        \"id\": \"ZoomIn\",\n        \"label\": \"Zoom In\"\n      },\n      {\n        \"id\": \"ZoomOut\",\n        \"label\": \"Zoom Out\"\n      },\n      {\n        \"id\": \"OriginalView\",\n        \"label\": \"Original View\"\n      },\n      null,\n      {\n        \"id\": \"Quality\"\n      },\n      {\n        \"id\": \"Pause\"\n      },\n      {\n        \"id\": \"Mute\"\n      },\n      null,\n      {\n        \"id\": \"Find\",\n        \"label\": \"Find...\"\n      },\n      {\n        \"id\": \"FindAgain\",\n        \"label\": \"Find Again\"\n      },\n      {\n        \"id\": \"Copy\"\n      },\n      {\n        \"id\": \"CopyAgain\",\n        \"label\": \"Copy Again\"\n      },\n      {\n        \"id\": \"CopySVG\",\n        \"label\": \"Copy SVG\"\n      },\n      {\n        \"id\": \"ViewSVG\",\n        \"label\": \"View SVG\"\n      },\n      {\n        \"id\": \"ViewSource\",\n        \"label\": \"View Source\"\n      },\n      {\n        \"id\": \"SaveAs\",\n        \"label\": \"Save As\"\n      },\n      null,\n      {\n        \"id\": \"Help\"\n      },\n      {\n        \"id\": \"About\",\n        \"label\": \"About Adobe CVG Viewer...\"\n      }\n    ]\n  }\n}";
	const size_t str_len = sizeof(str) - 1;
	const size_t bufferSize = (16 * 1024);
	const size_t finalBufferSize = bufferSize + sizeof(struct SeaArena);

	SEA_MALLOC_SCOPE(finalBufferSize) {

		struct SeaArena* arena = (struct SeaArena*) SEA_SCOPE_BUFFER;
		SeaArena.init(arena, SEA_SCOPE_BUFFER + sizeof(struct SeaArena), bufferSize);

		SEA_ARENA_SCOPE(arena) {

			struct SeaAllocator allocator = SeaArena.getAllocator(arena);
			printf("Remaining: %zu\n", SeaArena.remaining(arena));
			const struct SeaJsonValue* json = SeaJsonValue.ParseJson(str, str_len, &allocator);
			printf("Remaining: %zu\n", SeaArena.remaining(arena));

			if (json->type != SEA_JSON_OBJECT) { exit(12345678); }
			const struct SeaJsonValue* menu = SeaJsonObject.get(json->object, "menu");
			printf("Remaining: %zu\n", SeaArena.remaining(arena));
			if (menu->type != SEA_JSON_OBJECT) { exit(12349999); }
			char* abc_str = SeaJsonValue.toString(json, &allocator);
			printf("\n%s\n", abc_str);
			printf("Remaining: %zu\n", SeaArena.remaining(arena));

		}

	}

	puts("");
	puts("=========================================================================");
	puts("*   PERSON                                                              *");
	puts("=========================================================================");
	puts("");

	// ---- Arena -------------------------------------------------

	const size_t arenaSize = 128;
	SEA_MALLOC_SCOPE(arenaSize + sizeof(struct SeaArena)) {
		struct SeaArena* arena = (struct SeaArena*) SEA_SCOPE_BUFFER;
		SeaArena.init(arena, SEA_SCOPE_BUFFER + sizeof(struct SeaArena), arenaSize);
		for (int i = 0; i < 5; i++) {
			SEA_ARENA_SCOPE(arena) {
				arena_test(arena);
				puts("\n-----------------------------\n");
			}
		}
	}

	return 0;
}