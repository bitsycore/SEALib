#include "TestJsonGeneric.h"

#include <stdio.h>
#include <stdlib.h>

#include "SEA/Arena.h"
#include "SEA/JSONObject.h"
#include "SEA/JSONValue.h"
#include "../SEA/Utils/ScopeUtils.h"

void testJsonGeneric(void) {
    // ---- JSON -------------------------------------------------

    // language=JSON
    const char str[] =
            "{\n  \"menu\": {\n    \"header\": \"SVG Viewer\",\n    \"items\": [\n      {\n        \"id\": \"Open\"\n      },\n      {\n        \"id\": \"OpenNew\",\n        \"label\": \"Open New\"\n      },\n      null,\n      {\n        \"id\": \"ZoomIn\",\n        \"label\": \"Zoom In\"\n      },\n      {\n        \"id\": \"ZoomOut\",\n        \"label\": \"Zoom Out\"\n      },\n      {\n        \"id\": \"OriginalView\",\n        \"label\": \"Original View\"\n      },\n      null,\n      {\n        \"id\": \"Quality\"\n      },\n      {\n        \"id\": \"Pause\"\n      },\n      {\n        \"id\": \"Mute\"\n      },\n      null,\n      {\n        \"id\": \"Find\",\n        \"label\": \"Find...\"\n      },\n      {\n        \"id\": \"FindAgain\",\n        \"label\": \"Find Again\"\n      },\n      {\n        \"id\": \"Copy\"\n      },\n      {\n        \"id\": \"CopyAgain\",\n        \"label\": \"Copy Again\"\n      },\n      {\n        \"id\": \"CopySVG\",\n        \"label\": \"Copy SVG\"\n      },\n      {\n        \"id\": \"ViewSVG\",\n        \"label\": \"View SVG\"\n      },\n      {\n        \"id\": \"ViewSource\",\n        \"label\": \"View Source\"\n      },\n      {\n        \"id\": \"SaveAs\",\n        \"label\": \"Save As\"\n      },\n      null,\n      {\n        \"id\": \"Help\"\n      },\n      {\n        \"id\": \"About\",\n        \"label\": \"About Adobe CVG Viewer...\"\n      }\n    ]\n  }\n}";
    const size_t str_len = sizeof(str) - 1;
    const size_t bufferSize = (16 * 1024);
    const size_t finalBufferSize = bufferSize + sizeof(struct SEA_Arena);

    SEA_SCOPE_MALLOC(finalBufferSize) {
        struct SEA_Arena *arena = (struct SEA_Arena *) SEA_SCOPE_BUFFER;
        SEA_Arena_init(arena, SEA_SCOPE_BUFFER + sizeof(struct SEA_Arena), bufferSize);

        SEA_SCOPE_ARENA(arena) {
            struct SEA_Allocator allocator = SEA_Arena_allocator(arena);
            printf("Remaining: %zu\n", SEA_Arena_remaining(arena));
            struct SEA_JSONValue *json = SEA_JSONValue_FromString(str, str_len, &allocator);
            printf("Remaining: %zu\n", SEA_Arena_remaining(arena));

            if (json->type != SEA_JSON_OBJECT) { exit(12345678); }
            const struct SEA_JSONValue *menu = SEA_JSONObject_get(json->object, "menu");
            if (menu->type != SEA_JSON_OBJECT) { exit(12349999); }
            char *abc_str = SEA_JSONValue_toString(menu, &allocator);

            printf("\n%s\n\n", abc_str);
            printf("Remaining: %zu\n", SEA_Arena_remaining(arena));

            SEA_Allocator_free(&allocator, abc_str);
            SEA_JSONValue_free(json, &allocator);
        }
    }
}