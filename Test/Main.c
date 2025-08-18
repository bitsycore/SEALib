#include <stdio.h>

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

    struct SEA_ListDyn list = SEA_ListDyn(int, SEA_Allocator.Heap);

    SEA_ListDyn.add(&list, &(int){1});
    SEA_ListDyn.add(&list, &(int){2});
    SEA_ListDyn.add(&list, &(int){3});

    SEA_Iterator_foreach(int, SEA_ListDyn.iterator(&list, NULL)) {
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
