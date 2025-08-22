#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

#define TYPEOF __typeof__
#define TYPEOF_UNQUAL __typeof_unqual__

#ifdef var
#undef var
#endif
#define var __auto_type
#ifdef val
#undef val
#endif
#define val const __auto_type

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

#define SEA____NSEQ_N() \
        64, 63, 62, 61, 60, 59, 58, 57, 56, 55, \
        54, 53, 52, 51, 50, 49, 48, 47, 46, 45, \
        44, 43, 42, 41, 40, 39, 38, 37, 36, 35, \
        34, 33, 32, 31, 30, 29, 28, 27, 26, 25, \
        24, 23, 22, 21, 20, 19, 18, 17, 16, 15, \
        14, 13, 12, 11, 10,  9,  8,  7,  6,  5, \
         4,  3,  2,  1,  0
#define SEA___NARG_N(\
         _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
        _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
        _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
        _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
        _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
        _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
        _61, _62, _63, _64, N, ...) N
#define SEA___NARG_IMPL_(...) SEA___NARG_N(__VA_ARGS__)
#define SEA_NARG(...)  SEA___NARG_IMPL_(__VA_ARGS__, SEA____NSEQ_N())

#define SEA___CONCAT_IMPL(a, b) a##b
#define SEA_CONCAT(a, b) SEA___CONCAT_IMPL(a, b)
#define SEA___ARG1_IMPL(first, ...) first
#define SEA_ARG1(...) SEA___ARG1_IMPL(__VA_ARGS__)

#define List(type) union { \
    ListNode *head; \
    type *payload; \
}

ListNode* __listOfN(size_t size_element, size_t nb_elem, ...) {
    ListNode *head = NULL;
    ListNode *current = NULL;
    va_list args;
    va_start(args, nb_elem);
    for (size_t i = 0; i < nb_elem; i++) {
        ListNode *node = calloc(1, sizeof(ListNode) + size_element);
        if (head == NULL || current == NULL) {
            current = node;
            head = node;
        } else {
            current->next = node;
            current = node;
        }
        char* data_in = va_arg(args, char*);
        memcpy(node->data, &data_in, size_element);
    }
    va_end(args);
    return head;
}


#define listOfImpl(_first_, _name_list_, _type_, _nb_arg_, ...) ({ \
    List(_type_) _name_list_ = {0};    \
    if(0) { *(_name_list_.payload) = _first_; }                                                       \
    _name_list_.head = __listOfN(sizeof(_type_), _nb_arg_, __VA_ARGS__); \
    _name_list_;\
})

#define listOf(...) listOfImpl( \
    SEA_ARG1(__VA_ARGS__), \
    SEA_CONCAT(__list_, __COUNTER__),       \
    __typeof__(SEA_ARG1(__VA_ARGS__)), \
    SEA_NARG(__VA_ARGS__),               \
    __VA_ARGS__                         \
)

#define forEachImpl(list, type, name_node) \
    var name_node = (list)->head;          \
    for ( \
        struct { type value; size_t index; } __it = { (type)(list)->head->data, 0 }; \
        name_node != NULL; \
        name_node = name_node->next, name_node != NULL && (__it.value = (type)name_node->data) && (__it.index = __it.index+1 ) \
    )
#define forEach(list) forEachImpl(list, __typeof__((list)->payload), SEA_CONCAT(__foreach_node, __COUNTER__) )

#define forEachValue (*__it.value)
#define forEachIndex (__it.index)

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
    srand(time(NULL));

    Lazy(int) lazy = lazy_init(^{
        printf("Hello world\n");
        return 42;
    });

    //Lazy(int) lazy2 = lazy;
    lazy_set(&lazy, ^{ return 112; });

    test_arg(&lazy);

    val result1 = lazy_get(&lazy);
    __typeof_unqual(result1) result1_copy = result1;
    val result2 = lazy_get(&lazy);
    val result3 = lazy_get(&lazy);

    Lazy(int)* lazy_heaped = malloc(sizeof(Lazy(int)));
    lazy_copy(lazy_heaped, &lazy);
    val result4 = lazy_get(lazy_heaped);
    val result5 = lazy_get(lazy_heaped);
    val result6 = lazy_get(lazy_heaped);

    free(lazy_heaped);


    val random = rand() % RAND_MAX;

    printf("Hello World!\n");

    var players = listOf(
            &((struct Player) {"Alice", 30}),
            &((struct Player) {"Bob", 12}),
            &((struct Player) {"Charlie", 25}),
            &((struct Player) {"Elmo", 11})
    );

    forEach(&players) {
        printf("[%zu] : %s is %d years old\n", forEachIndex, forEachValue->name, forEachValue->age);
    }

    var abcde = listOf(1,2,3,4,5,6,7);

    forEach(&abcde) {
        printf("%d\n", forEachValue);
    }

    return 0;
}