#include "TestArena.h"

#include <stdio.h>
#include <stdlib.h>

#include "Person.h"
#include "SEA/Arena.h"
#include "SEA/Random.h"
#include "../SEA/Utils/ScopeUtils.h"

static const char *randomName() {
    switch (SEA_Random.Uint64() % 16) {
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

void testArena(void) {
    const size_t arenaSize = 512;
    SEA_SCOPE_MALLOC(arenaSize + sizeof(struct SEA_Arena)) {
        struct SEA_Arena *arena = (struct SEA_Arena *) SEA_SCOPE_BUFFER;
        SEA_Arena.init(arena, SEA_SCOPE_BUFFER + sizeof(struct SEA_Arena), arenaSize);
        for (int i = 0; i < 5; i++) {
            SEA_SCOPE_ARENA(arena) {
                const struct SEA_Allocator arena_allocator = SEA_Arena.allocator(arena);

                struct Person *me = SEA_Arena.alloc(arena, sizeof(struct Person));
                Person.initWithName(me, randomName());

                struct Person *you = &(struct Person){};
                Person.init(you, randomName(), (int) (SEA_Random.Uint64() % 100));

                const struct Person maybeMe = *me;

                const char *meStr = Person.toString(me, &arena_allocator);
                puts(meStr);

                const char *youStr = Person.toString(you, &arena_allocator);
                puts(youStr);

                if (Person.equals(me, you)) {
                    puts("They are equal!");
                } else {
                    puts("They are not equal!");
                }

                if (Person.equals(me, &maybeMe)) {
                    puts("They are equal!");
                } else {
                    puts("They are not equal!");
                }

                printf("Remaining: %zu\n", SEA_Arena.remaining(arena));
            }
            puts("\n-----------------------------\n");
        }
    }
}