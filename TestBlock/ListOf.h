#ifndef SEALIB_LISTOF_H
#define SEALIB_LISTOF_H

#include "Macro.h"

#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode ListNode;
struct ListNode {
	ListNode *next;
	char data[];
};

#define List(type) union { \
    ListNode *head; \
    type *payload; \
}

static inline ListNode* ___listOfN(size_t size_element, size_t nb_elem, ...) {
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

#endif //SEALIB_LISTOF_H
