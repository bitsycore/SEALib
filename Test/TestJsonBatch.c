#include "TestJsonBatch.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SEA/Arena.h"
#include "SEA/JSONValue.h"
#include "../SEA/Utils/ScopeUtils.h"

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
    SEA_SCOPE_MALLOC(finalBufferSize) {
        struct SEA_Arena *arena = (struct SEA_Arena *) SEA_SCOPE_BUFFER;
        SEA_Arena.init(arena, SEA_SCOPE_BUFFER + sizeof(struct SEA_Arena), bufferSize);
        struct SEA_Allocator allocator = SEA_Arena.allocator(arena);
        for (size_t i = 0; i < sizeof(json_tests) / sizeof(json_tests[0]); i++) {
            SEA_SCOPE_ARENA(arena) {
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