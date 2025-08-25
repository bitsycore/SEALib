#include "Lazy.h"
#include "Macro.h"
#include "ListOf.h"
#include "ForEach.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define TEST_TITLE(x__) \
	puts("\n================================================"); \
	puts("               Testing " x__ ); \
	puts("================================================\n");

struct Player {
	const char* name;
	int age;
};

void test_lazy() {
	TEST_TITLE("Lazy")

	Lazy(int) lazy = Lazy_init(^{
		puts("Lazy evaluated");
		return 42;
	});

	val result1 = Lazy_get(&lazy);
	val result2 = Lazy_get(&lazy);
	val result3 = Lazy_get(&lazy);

	Lazy_set(&lazy, ^{ return 112; });

	Lazy(int)* lazy_heaped = malloc(sizeof(Lazy(int)));
	Lazy_copy(lazy_heaped, &lazy);

	val result4 = Lazy_get(lazy_heaped);
	val result5 = Lazy_get(lazy_heaped);
	val result6 = Lazy_get(lazy_heaped);

	free(lazy_heaped);
}

void test_listof() {
	TEST_TITLE("listOf")

	puts(" --- listOf struct --- \n");

	var players = listOf(
		((struct Player) {"Alice", 30}),
		((struct Player) {"Bob", 12}),
		((struct Player) {"Charlie", 25}),
		((struct Player) {"Elmo", 28}),
		((struct Player) {"Bob", 17}),
		((struct Player) {"Joe", 11}),
		((struct Player) {"Morice", 11}),
		((struct Player) {"Ami", 21}),
		((struct Player) {"Tamile", 54}),
		((struct Player) {"Edo", 66})
	);

	printf("Size: %zu\n", players.list->size);

	forEach(&players) {
		printf("[%zu] : %s is %d years old\n", forEachIndex, forEachValue.name, forEachValue.age);
	}

	puts("\n --- listOf struct ref --- \n");

	var players2 = listOf(
		listGet(&players, 0),
		listGet(&players, 1),
		listGet(&players, 2),
		listGet(&players, 3)
		);

	forEach(&players2) {
		printf("[%zu] : %s is %d years old\n", forEachIndex, forEachValue->name, forEachValue->age);
	}

	listFree(&players);
	listFree(&players2);

	puts("\n --- listOf int --- \n");

	var abcde = listOf(1,2,3,4,5,6,7);
	forEach(&abcde) {
		printf("%d, ", forEachValue);
	}

	listFree(&abcde);
}

int main() {
    srand(time(NULL));
	test_lazy();
	test_listof();
    return 0;
}