#include "TestArraySegmented.h"

#include <stdio.h>

#include "SEA/Allocator.h"
#include "SEA/ArraySegmented.h"

void testArraySegmented(void) {
	typedef struct Test {
		int a;
		int b;
	} TestType;

	struct SEA_ArraySegmented arr = SEA_ArraySegmented(TestType, NULL);
	for (int i = 0; i < 10000; i++) {
		SEA_ArraySegmented.add(&arr, &(TestType){i, i + 1});
	}
	printf("Size: %zu\n", SEA_ArraySegmented.count(&arr));

	SEA_ArraySegmented_foreach(TestType, &arr) {
		if (it.index % 50 == 0)
			printf("Index: %zu: TestType(%d, %d)\n", it.index, it.value->a, it.value->b);
	}

	SEA_ArraySegmented.free(&arr);

	printf("Size: %zu\n", SEA_ArraySegmented.count(&arr));
	SEA_ArraySegmented_foreach(TestType, &arr) {
		if (it.index % 50 == 0)
			printf("Index: %zu: TestType(%d, %d)\n", it.index, it.value->a, it.value->b);
	}
}