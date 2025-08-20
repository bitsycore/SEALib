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

int foo(int x) __attribute__((overloadable)) {
	return x + 1;
}

float foo(float x) __attribute__((overloadable)) {
	return x + 1.5f;
}

typedef int v4si __attribute__((__vector_size__(sizeof(int) * 4)));
typedef float float4 __attribute__((ext_vector_type(sizeof(float) * 4)));;
typedef float float2 __attribute__((ext_vector_type(sizeof(float) * 2)));

union IntOrFloat {
	int i;
	float f;
} __attribute__((transparent_union)) ;

void handle_int(int x) {
	printf("Handling as int: %d\n", x);
}

void handle_float(float x) {
	printf("Handling as float: %f\n", x);
}

#define CONCAT(a, b) a##b

#define MACRO(x) \
    MACRO_IMPL(x, __COUNTER__)

#define MACRO_IMPL(x, n) \
    __auto_type CONCAT(if_int, n) = (union IntOrFloat){.i = (int)(x)}; \
    __auto_type CONCAT(if_float, n) = (union IntOrFloat){.f = (float)(x)}; \
    _Generic((x), int: handle_int, float: handle_float)(x)


int main() {

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

	MACRO(42);
	MACRO(3.0f);
	MACRO(2.1f);
	MACRO(55);

	printf("%d\n", __COUNTER__);
	printf("%d\n", __COUNTER__);
	printf("%d\n", __COUNTER__);
	printf("%d\n", __COUNTER__);
	printf("%d\n", __COUNTER__);
	printf("%d\n", __COUNTER__);

	__auto_type hello = ^int(int abc) {
		return foo(4) + foo(4.0f) + abc;
	};
	__auto_type block = Block_copy(hello);
	block(5);
	Block_release(block);

	int abc = 5;
	int* _Nonnull acc = &abc;


	//abcdef((union IntOrFloat){.i = 42});
	v4si a;

	v4si abcd = {1, 2, 3, 4};
	v4si bef = {10, 20, 30, 40};
	v4si c = abcd + bef;  // Element-wise addition

	for (int i = 0; i < 4; i++) {
		printf("c[%d] = %d\n", i, c[i]);
	}

	acc = NULL;

	__auto_type asbcdef = hello(512);


	v4si vsi = (v4si){1, 2, 3, 4};
	float4 vf = (float4)(1.0f, 2.0f, 3.0f, 4.0f);

	printf("%d\n", asbcdef);

	return 0;

    struct SEA_ListDyn *list = &SEA_ListDyn(int, SEA_Allocator.Heap);

    SEA_ListDyn.reserve(list, 512);

    for (int i = 0; i < 5; i++) {
        SEA_ListDyn.add(list, &(int){1});
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
