#include <stdio.h>
#include <string.h>

#include "TestArena.h"
#include "TestJsonBatch.h"
#include "TestJsonGeneric.h"
#include "TestJsonNumbers.h"
#include "TestLists.h"
#include "SEA/ListDyn.h"

#include "BlockRuntime/Block.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif

int main() {

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    ListDyn *list = &ListDyn(int, Allocator_Heap);

    ListDyn_reserve(list, 512);

    for (int i = 0; i < 5; i++) {
        ListDyn_add(list, &(int){1});
        (*(int*)ListDyn_alloc(list)) = 2;
        ListDyn_add(list, &(int){3});
        ListDyn_add(list, &(int){4});
        ListDyn_insert(list, 2, &(int){999});
        ListDyn_remove(list, 0);
    }

    ListDyn_shrink(list);

    printf("List size: %zu\n", ListDyn_count(list));
    printf("List capacity: %zu\n", ListDyn_capacity(list));

    Iterator_foreach(int, ListDyn_iterator(list, NULL)) {
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
