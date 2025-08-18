#include <stdio.h>
#include <string.h>

#include "TestArena.h"
#include "TestJsonBatch.h"
#include "TestJsonGeneric.h"
#include "TestJsonNumbers.h"
#include "TestLists.h"
#include "SEA/ListDyn.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif

int main() {

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    struct SEA_ListDyn *list = &SEA_ListDyn(int, SEA_Allocator.Heap);

    SEA_ListDyn.reserve(list, 512);

    for (int i = 0; i < 5; i++) {
        SEA_ListDyn.add(list, &(int){1});
        // Cheaper version, is compiler enough smart to make it itself ?
        (*(int*)SEA_ListDyn.alloc(list)) = 2;
        SEA_ListDyn.add(list, &(int){3});
        SEA_ListDyn.add(list, &(int){4});
        SEA_ListDyn.insert(list, 2, &(int){999});
        SEA_ListDyn.remove(list, 0);
    }

    SEA_ListDyn.shrink(list);

    printf("List size: %zu\n", SEA_ListDyn.count(list));
    printf("List capacity: %zu\n", SEA_ListDyn.capacity(list));

    SEA_Iterator_foreach(int, SEA_ListDyn.iterator(list, NULL)) {
        printf("%d\n", *it.value);
    }

    return 0;

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
