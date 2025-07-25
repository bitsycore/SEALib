// ============================================
// File: Main.sea
// ============================================

#include "Person.h"
#include "Arena.h"
#include "Random.h"

#include <malloc.h>
#include <stdio.h>
#include <stdalign.h>

const char* randomName() {
    uint64_t random = Random.randUint64() % 16;
    switch (random) {
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
        case 15: return "Martin";
        default: return "Unknown";
    }
}

void arena_test(struct Arena* arena) {
    struct Person* me = Arena.alloc(arena, sizeof(struct Person), alignof(struct Person));
    Person.initWithName(me, randomName());

    struct Person* you = Arena.alloc(arena, sizeof(struct Person), alignof(struct Person));
    Person.init(you, randomName(), (int) (Random.randUint64() % 100));

    size_t meSize = Person.toStringSize(me);
    char* meStr = Person.toString(me, Arena.alloc(arena, meSize, alignof(char)), meSize);
    puts(meStr);

    size_t youSize = Person.toStringSize(you);
    char* youStr = Person.toString(you, Arena.alloc(arena, youSize, alignof(char)), youSize);
    puts(youStr);

    if (Person.equals(me, you)) {
        puts("They are equal!");
    } else {
        puts("They are not equal!");
    }

    printf("Remaining: %zu\n", Arena.remaining(arena));
}

int main() {
    struct Arena arena;
    size_t arenaSize = 256;
    Arena.init(&arena, malloc(arenaSize), arenaSize);
    for (int i = 0; i < 10; i++) {
        arena_test(&arena);
        Arena.reset(&arena);
        puts("-----------------------------");
    }
    free(arena.buffer);
    return 0;
}