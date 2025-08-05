#include "Person.h"

#include <SEA/Allocator.h>
#include <SEA/Arena.h>
#include <SEA/JSONObject.h>
#include <SEA/JSONValue.h>
#include <SEA/Utils/MemoryUtils.h>
#include <SEA/Random.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TestJsonNumbers.h"

const char *randomName() {
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

void testJsonGeneric(void) {
    // ---- JSON -------------------------------------------------

    // language=JSON
    const char str[] =
            "{\n  \"menu\": {\n    \"header\": \"SVG Viewer\",\n    \"items\": [\n      {\n        \"id\": \"Open\"\n      },\n      {\n        \"id\": \"OpenNew\",\n        \"label\": \"Open New\"\n      },\n      null,\n      {\n        \"id\": \"ZoomIn\",\n        \"label\": \"Zoom In\"\n      },\n      {\n        \"id\": \"ZoomOut\",\n        \"label\": \"Zoom Out\"\n      },\n      {\n        \"id\": \"OriginalView\",\n        \"label\": \"Original View\"\n      },\n      null,\n      {\n        \"id\": \"Quality\"\n      },\n      {\n        \"id\": \"Pause\"\n      },\n      {\n        \"id\": \"Mute\"\n      },\n      null,\n      {\n        \"id\": \"Find\",\n        \"label\": \"Find...\"\n      },\n      {\n        \"id\": \"FindAgain\",\n        \"label\": \"Find Again\"\n      },\n      {\n        \"id\": \"Copy\"\n      },\n      {\n        \"id\": \"CopyAgain\",\n        \"label\": \"Copy Again\"\n      },\n      {\n        \"id\": \"CopySVG\",\n        \"label\": \"Copy SVG\"\n      },\n      {\n        \"id\": \"ViewSVG\",\n        \"label\": \"View SVG\"\n      },\n      {\n        \"id\": \"ViewSource\",\n        \"label\": \"View Source\"\n      },\n      {\n        \"id\": \"SaveAs\",\n        \"label\": \"Save As\"\n      },\n      null,\n      {\n        \"id\": \"Help\"\n      },\n      {\n        \"id\": \"About\",\n        \"label\": \"About Adobe CVG Viewer...\"\n      }\n    ]\n  }\n}";
    const size_t str_len = sizeof(str) - 1;
    const size_t bufferSize = (16 * 1024);
    const size_t finalBufferSize = bufferSize + sizeof(struct SEA_Arena);

    SEA_MALLOC_SCOPE(finalBufferSize) {
        struct SEA_Arena *arena = (struct SEA_Arena *) SEA_SCOPE_BUFFER;
        SEA_Arena.init(arena, SEA_SCOPE_BUFFER + sizeof(struct SEA_Arena), bufferSize);

        SEA_ARENA_SCOPE(arena) {
            struct SEA_Allocator allocator = SEA_Arena.allocator(arena);
            printf("Remaining: %zu\n", SEA_Arena.remaining(arena));
            struct SEA_JSONValue *json = SEA_JSONValue.FromString(str, str_len, &allocator);
            printf("Remaining: %zu\n", SEA_Arena.remaining(arena));

            if (json->type != SEA_JSON_OBJECT) { exit(12345678); }
            const struct SEA_JSONValue *menu = SEA_JSONObject.get(json->object, "menu");
            if (menu->type != SEA_JSON_OBJECT) { exit(12349999); }
            char *abc_str = SEA_JSONValue.toString(menu, &allocator);

            printf("\n%s\n\n", abc_str);
            printf("Remaining: %zu\n", SEA_Arena.remaining(arena));

            SEA_Allocator.free(&allocator, abc_str);
            SEA_JSONValue.free(json, &allocator);
        }
    }
}

void testArena(void) {
    const size_t arenaSize = 512;
    SEA_MALLOC_SCOPE(arenaSize + sizeof(struct SEA_Arena)) {
        struct SEA_Arena *arena = (struct SEA_Arena *) SEA_SCOPE_BUFFER;
        SEA_Arena.init(arena, SEA_SCOPE_BUFFER + sizeof(struct SEA_Arena), arenaSize);
        for (int i = 0; i < 5; i++) {
            SEA_ARENA_SCOPE(arena) {
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

static const char *json_tests[] = {
    // 1. Minimal object
    "{}",

    // 2. Minimal array
    "[]",

    // 3. Empty string
    "\"\"",

    "[{}]",

    // 2. Nested arrays & objects
    "{\n"
    "  \"a\": [1, {\"b\": [true, false, null]}, 3],\n"
    "  \"c\": {\"d\": {\"e\": \"deep\"}}\n"
    "}",

    // 3. Escaped characters
    "{\n"
    "  \"text\": \"Line1\\nLine2\\tTabbed\\\\Backslash\\\"Quote\",\n"
    "  \"unicode\": \"\\uD83D\\uDE00\"\n"
    "}",

    // 4. Empty structures
    "{\n"
    "  \"emptyObj\": {},\n"
    "  \"emptyArr\": []\n"
    "}",

    // 5. Large numbers
    "{\n"
    "  \"big\": 12345678901234567890,\n"
    "  \"small\": -0.000000123\n"
    "}",

    // 6. Mixed whitespace
    "{   \"a\"  :  [  1 ,  2  ,3]   }",

    // 7. Deep nesting
    "{\n"
    "  \"a\": {\"b\": {\"c\": {\"d\": {\"e\": {\"f\": null}}}}}\n"
    "}",

    // 8. Mixed types
    "[1, \"two\", true, false, null, {\"x\": 1}, [2,3]]",

    // 9. Keys with empty string
    "{\n"
    "  \"\": 1,\n"
    "  \"normal\": 2\n"
    "}",

    // 10. Unicode line separator
    "{\n"
    "  \"u\": \"\\u2028\\u2029\"\n"
    "}"
};

void testJsonBatch(void) {
    const size_t bufferSize = 16 * 1024;
    const size_t finalBufferSize = bufferSize + sizeof(struct SEA_Arena);
    SEA_MALLOC_SCOPE(finalBufferSize) {
        struct SEA_Arena *arena = (struct SEA_Arena *) SEA_SCOPE_BUFFER;
        SEA_Arena.init(arena, SEA_SCOPE_BUFFER + sizeof(struct SEA_Arena), bufferSize);
        struct SEA_Allocator allocator = SEA_Arena.allocator(arena);
        for (size_t i = 0; i < sizeof(json_tests) / sizeof(json_tests[0]); i++) {
            SEA_ARENA_SCOPE(arena) {
                const char *str = json_tests[i];
                const size_t str_len = strlen(str);

                printf("\n---- Test #%zu ----\n", i + 1);

                struct SEA_JSONValue *json = SEA_JSONValue.FromString(str, str_len, &allocator);
                if (!json) {
                    printf("Parse error\n");
                    continue;
                }

                char *out_str = SEA_JSONValue.toString(json, &allocator);
                printf("%s\n", out_str);

                SEA_Allocator.free(&allocator, out_str);
                SEA_JSONValue.free(json, &allocator);
            }
        }
    }
}

int main() {

    puts("");
    puts("=========================================================================");
    puts("*   JSON Numbers                                                        *");
    puts("=========================================================================");
    puts("");

    testJsonNumbersParsing();

    puts("");
    puts("=========================================================================");
    puts("*   JSON Generic                                                        *");
    puts("=========================================================================");
    puts("");

    testJsonGeneric();

    puts("");
    puts("=========================================================================");
    puts("*   ARENA PERSON                                                        *");
    puts("=========================================================================");
    puts("");

    testArena();

    puts("");
    puts("=========================================================================");
    puts("*   MORE JSON TEST                                                      *");
    puts("=========================================================================");
    puts("");

    testJsonBatch();

    return 0;
}