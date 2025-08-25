#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include "Lazy.h"
#include "Macro.h"

#define typeOf __typeof__
#define typeOfUn __typeof_unqual__
#define var __auto_type
#define val const __auto_type

// =====================================
// MARK: List
// =====================================

typedef struct ListNode ListNode;
struct ListNode {
	ListNode *next;
	char data[];
};

#define List(type) union { \
    ListNode *head; \
    type *payload; \
}


struct Player {
	char name[100];
	int age;
};

// =====================================
// MARK: listOf
// =====================================

ListNode* ___listOfN(size_t size_element, size_t nb_elem, ...) {
	printf("size_element = %zu\n", size_element);
    ListNode *head = NULL;
    ListNode *current = NULL;
    va_list args;
    va_start(args, nb_elem);
	val size = (sizeof(ListNode) + size_element);
	var buffer = (char*)calloc(nb_elem, size);
    for (size_t i = 0; i < nb_elem; i++) {
        ListNode *node = (ListNode *)(buffer + size * i);
        if (head == NULL || current == NULL) {
            current = node;
            head = node;
        } else {
            current->next = node;
            current = node;
        }
        void* data_in = va_arg(args, void*);
        memcpy(node->data, data_in, size_element);
    }
    va_end(args);
    return head;
}

#define freeListOf(list) free((list)->head)

#define ___listOfImpl(_name_list_, _type_, _nb_arg_, ...) ({ \
    List(_type_) _name_list_ = {0};    \
    _name_list_.head = ___listOfN(sizeof(_type_), _nb_arg_, __VA_ARGS__); \
    _name_list_;\
})

#define ___listOfHolder(x) ({ \
    static typeOf(x) holder = x;           \
    &holder;                        \
})

#define listOf(_first_, ...) ___listOfImpl(                 \
    SEA_PP_CONCAT(__list_, __COUNTER__),                 \
    typeOf(_first_),                                     \
    SEA_PP_NARG(__VA_ARGS__) + 1,                        \
    SEA_MAP(___listOfHolder, _first_, __VA_ARGS__) \
)

// =====================================
// MARK: forEach
// =====================================

#define forEachImpl(list, type, name_node) \
    var name_node = (list)->head;          \
    for ( \
        struct { type value; size_t index; } __it = { (type)(list)->head->data, 0 }; \
        name_node != NULL; \
        name_node = name_node->next, name_node != NULL && (__it.value = (type)name_node->data) && (__it.index = __it.index+1 ) \
    )

#define forEach(list) forEachImpl(list, typeOf((list)->payload), SEA_PP_CONCAT(__foreach_node, __COUNTER__) )
#define forEachValue (*__it.value)
#define forEachIndex (__it.index)

#define TEST_TITLE(x__) \
puts("\n================================================"); \
puts("               Testing " x__ ); \
puts("================================================\n");

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
		((struct Player) {"Elmo", 11})
	);

	forEach(&players) {
		printf("[%zu] : %s is %d years old\n", forEachIndex, forEachValue.name, forEachValue.age);
	}

	freeListOf(&players);

	puts("\n --- listOf int --- \n");

	var abcde = listOf(1,2,3,4,5,6,7);

	forEach(&abcde) {
		printf("%d, ", forEachValue);
	}

	freeListOf(&abcde);
}

int main() {
    srand(time(NULL));
	test_lazy();
	test_listof();
    return 0;
}