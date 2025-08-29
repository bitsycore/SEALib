#include <stdio.h>

#include "TestArena.h"
#include "TestJsonBatch.h"
#include "TestJsonGeneric.h"
#include "TestJsonNumbers.h"
#include "TestLists.h"
#include "SEA/ListDyn.h"
#include "SEA/Compat/TypeCompat.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif

int main() {

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    SEA_VAL list = &SEA_ListDyn(int, NULL);

    SEA_ListDyn_reserve(list, 512);
    for (int i = 0; i < 5; i++) {
        SEA_ListDyn_add(list, &(int){1});
        *(int*)SEA_ListDyn_alloc(list) = 2;
        SEA_ListDyn_add(list, &(int){3});
        SEA_ListDyn_add(list, &(int){4});
        SEA_ListDyn_insert(list, 2, &(int){999});
        SEA_ListDyn_remove(list, 0);
    }

    SEA_ListDyn_shrink(list);

    printf("List size: %zu\n", SEA_ListDyn_count(list));
    printf("List capacity: %zu\n", SEA_ListDyn_capacity(list));

    SEA_Iterator_foreach(int, SEA_ListDyn_iterator(list, NULL)) {
        printf("%d\n", *it.value);
    }

    test_lists_all();

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
