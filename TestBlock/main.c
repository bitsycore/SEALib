#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

#ifdef var
#undef var
#endif
#define var __auto_type
#ifdef val
#undef val
#endif
#define val const __auto_type

#define PP_NARG(...)  PP_NARG_(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
         _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
        _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
        _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
        _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
        _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
        _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
        _61, _62, _63, _64, N, ...) N

#define PP_RSEQ_N() \
        64, 63, 62, 61, 60, 59, 58, 57, 56, 55, \
        54, 53, 52, 51, 50, 49, 48, 47, 46, 45, \
        44, 43, 42, 41, 40, 39, 38, 37, 36, 35, \
        34, 33, 32, 31, 30, 29, 28, 27, 26, 25, \
        24, 23, 22, 21, 20, 19, 18, 17, 16, 15, \
        14, 13, 12, 11, 10,  9,  8,  7,  6,  5, \
         4,  3,  2,  1,  0


#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)
#define FIRST_ARG(first, ...) first
#define GET_FIRST(...) FIRST_ARG(__VA_ARGS__)

typedef struct ListNode ListNode;
struct ListNode {
    ListNode *next;
    char data[];
};

#define List(type) union { \
    ListNode *head; \
    type *payload; \
}

ListNode* _listOfN(size_t size_element, size_t nb_elem, char* elem1, char* elem2, char* elem3, char* elem4, char* elem5, char* elem6, char* elem7, char* elem8, char* elem9, char* elem10, char* elem11, char* elem12) {
    ListNode *head = NULL;
    ListNode *current = NULL;
    for (size_t i = 0; i < nb_elem; i++) {
        ListNode *node = calloc(1, sizeof(ListNode) + size_element);
        if (head == NULL || current == NULL) {
            current = node;
            head = node;
        } else {
            current->next = node;
            current = node;
        }
        switch (i) {
            case 0:memcpy(node->data, &elem1, size_element);
                break;
            case 1:memcpy(node->data, &elem2, size_element);
                break;
            case 2:memcpy(node->data, &elem3, size_element);
                break;
            case 3:memcpy(node->data, &elem4, size_element);
                break;
            case 4:memcpy(node->data, &elem5, size_element);
                break;
            case 5:memcpy(node->data, &elem6, size_element);
                break;
            case 6:memcpy(node->data, &elem7, size_element);
                break;
            case 7:memcpy(node->data, &elem8, size_element);
                break;
            case 8:memcpy(node->data, &elem9, size_element);
                break;
            case 9:memcpy(node->data, &elem10, size_element);
                break;
            case 10:memcpy(node->data, &elem11, size_element);
                break;
            case 11:memcpy(node->data, &elem12, size_element);
                break;
        }

    }
    return head;
}

#define _listOf1(size_element, ele1 ) _listOfN(size_element, 1, (char*)ele1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define _listOf2(size_element, ele1, ele2 ) _listOfN(size_element, 2, (char*)ele1, (char*)ele2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define _listOf3(size_element, ele1, ele2, ele3 ) _listOfN(size_element, 3, (char*)ele1, (char*)ele2, (char*)ele3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define _listOf4(size_element, ele1, ele2, ele3, ele4 ) _listOfN(size_element, 4, (char*)ele1, (char*)ele2, (char*)ele3, (char*)ele4, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define _listOf5(size_element, ele1, ele2, ele3, ele4, ele5 ) _listOfN(size_element, 5, (char*)ele1, (char*)ele2, (char*)ele3, (char*)ele4, (char*)ele5, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define _listOf6(size_element, ele1, ele2, ele3, ele4, ele5, ele6 ) _listOfN(size_element, 6, (char*)ele1, (char*)ele2, (char*)ele3, (char*)ele4, (char*)ele5, (char*)ele6, NULL, NULL, NULL, NULL, NULL, NULL)
#define _listOf7(size_element, ele1, ele2, ele3, ele4, ele5, ele6, ele7 ) _listOfN(size_element, 7, (char*)ele1, (char*)ele2, (char*)ele3, (char*)ele4, (char*)ele5, (char*)ele6, (char*)ele7, NULL, NULL, NULL, NULL, NULL)
#define _listOf8(size_element, ele1, ele2, ele3, ele4, ele5, ele6, ele7, ele8 ) _listOfN(size_element, 8, (char*)ele1, (char*)ele2, (char*)ele3, (char*)ele4, (char*)ele5, (char*)ele6, (char*)ele7, (char*)ele8, NULL, NULL, NULL, NULL)
#define _listOf9(size_element, ele1, ele2, ele3, ele4, ele5, ele6, ele7, ele8, ele9 ) _listOfN(size_element, 9, (char*)ele1, (char*)ele2, (char*)ele3, (char*)ele4, (char*)ele5, (char*)ele6, (char*)ele7, (char*)ele8, (char*)ele9, NULL, NULL, NULL)
#define _listOf10 size_element, ele1, ele2, ele3, ele4, ele5, ele6, ele7, ele8, ele9, ele10 ) _listOfN(size_element, 10, (char*)ele1, (char*)ele2, (char*)ele3, (char*)ele4, (char*)ele5, (char*)ele6, (char*)ele7, (char*)ele8, (char*)ele9, (char*)ele10, NULL, NULL)
#define _listOf11 size_element, ele1, ele2, ele3, ele4, ele5, ele6, ele7, ele8, ele9, ele10, ele11 ) _listOfN(size_element, 11, (char*)ele1, (char*)ele2, (char*)ele3, (char*)ele4, (char*)ele5, (char*)ele6, (char*)ele7, (char*)ele8, (char*)ele9, (char*)ele10, (char*)ele11, NULL)
#define _listOf12 size_element, ele1, ele2, ele3, ele4, ele5, ele6, ele7, ele8, ele9, ele10, ele11, ele12 ) _listOfN(size_element, 12, (char*)ele1, (char*)ele2, (char*)ele3, (char*)ele4, (char*)ele5, (char*)ele6, (char*)ele7, (char*)ele8, (char*)ele9, (char*)ele10, (char*)ele11, (char*)ele12)

#define _listOfSelector(size_element, n, ...) \
    _listOf##n(size_element, __VA_ARGS__)

#define listOfImpl(first, name_list, type, nb_arg, ...) ({ \
    List(type) name_list = {0};    \
    if(0) {                                                       \
        *(name_list.payload) = first;     \
    }                                                       \
    name_list.head = _listOfSelector(sizeof(type), nb_arg, __VA_ARGS__); \
    name_list;\
})


#define listOf(...) listOfImpl( GET_FIRST(__VA_ARGS__), CONCAT(__list_, __COUNTER__), __typeof__(GET_FIRST(__VA_ARGS__)), PP_NARG(__VA_ARGS__), __VA_ARGS__)

#define forEachImpl(list, type, name_node) \
    var name_node = (list)->head;          \
    for ( \
        type __it = (type)(list)->head->data; \
        name_node != NULL; \
        name_node = name_node->next, name_node != NULL && (__it = (type)name_node->data) \
    )
#define forEach(list) forEachImpl(list, __typeof__((list)->payload), CONCAT(__foreach_node, __COUNTER__) )

#define it (*__it)

struct Player {
    char name[100];
    int age;
};

#define lazy_init(_f) { .f = _f, .done = false, }
#define lazy_set(lazy_, _f) ({ (lazy_)->f = _f; (lazy_)->done = false; })

#define lazy_get(lazy_) ( __typeof__((lazy_)->result) ) ({ \
	if (!(lazy_)->done) { (lazy_)->result = (lazy_)->f(); (lazy_)->done = true; }  \
	(lazy_)->result; \
})
#define lazy_copy(lazy_, arg) ({ memcpy((lazy_), (arg), sizeof(*(lazy_))); })

#define Lazy(type) \
    struct  { \
        bool done; \
        type (^f)(); \
        type result; \
    }




void test_arg(Lazy(int)* arg) {
	val a = lazy_get(arg);
	printf("%d\n", a);
}

int main() {
	Lazy(int) lazy = lazy_init(^{
		printf("Hello world\n");
		return 42;
	});

	//Lazy(int) lazy2 = lazy;
	lazy_set(&lazy, ^{ return 112; });

	// Lazy(int) lazy2 = lazy;
	// Lazy(float) lazy3 = lazy;

	test_arg(&lazy);

	val result1 = lazy_get(&lazy);
	val result2 = lazy_get(&lazy);
	val result3 = lazy_get(&lazy);

	Lazy(int)* lazy_heaped = malloc(sizeof(Lazy(int)));
	lazy_copy(lazy_heaped, &lazy);
	val result4 = lazy_get(lazy_heaped);
	val result5 = lazy_get(lazy_heaped);
	val result6 = lazy_get(lazy_heaped);

	free(lazy_heaped);
//
//	val result7 = lazy_get(lazy2);
//	val result8 = lazy_get(lazy2);
//	val result9 = lazy_get(lazy2);



    var players = listOf(
        &((struct Player) {"Alice", 30}),
        &((struct Player) {"Bob", 12}),
        &((struct Player) {"Charlie", 25}),
        &((struct Player) {"Elmo", 11})
    );

    forEach(&players) {
        printf("%s is %d years old\n", it->name, it->age);
    }

    var abcde = listOf(1,2,3,4,5,6,7);
    forEach(&abcde) {
        printf("%d\n", it);
    }

    //val d = listOf(1, 2, 3, 4, 5);
    val abc = ^(int a){
        return a + 42;
    };

    val hello = abc(8);

    printf("%d\n", hello);

    return 0;
}