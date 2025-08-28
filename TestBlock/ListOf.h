#ifndef SEALIB_LISTOF_H
#define SEALIB_LISTOF_H

#include "Macro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListData {
	size_t size;
	size_t elementSize;
	char data[];
} ListData;

#define List(_type_) union { \
    ListData *list; \
    _type_ *payload; \
}

static inline ListData* ___listOfN(const size_t elementSize, const size_t numberOfElement, char* array) {
	val sizeToAlloc = sizeof(ListData) + (elementSize * numberOfElement);
	printf("elementSize: %zu, numberOfElement: %zu, sizeToAlloc: %zu\n", elementSize, numberOfElement, sizeToAlloc);
	ListData* list = malloc(sizeToAlloc);
	list->size = numberOfElement;
	list->elementSize = elementSize;
	char* buffer = list->data;
	for (size_t i = 0; i < numberOfElement; i++) {
		char* dataIn = array + (elementSize * i);
		memcpy(buffer + (elementSize * i), dataIn, elementSize);
	}
	return list;
}

#define ___listOfImpl(_name_list_, _type_, _nb_arg_, ...) ({ \
    List(_type_) _name_list_ = {0};    \
    _name_list_.list = ___listOfN(sizeof(_type_), _nb_arg_, (char*)((_type_[]) { __VA_ARGS__ } )); \
    _name_list_;\
})


#define listOf(_first_, ...) ___listOfImpl(             \
    SEA_PP_CONCAT(__list_, __COUNTER__),                \
    typeOf(_first_),                                    \
    SEA_PP_NARG(__VA_ARGS__) + 1,                       \
    _first_, __VA_ARGS__ 								\
)

#define listFree( _list_ ) free( (_list_)->list )

#define listGet(_list_, _index_) ( \
	( typeOf( (_list_)->payload ) ) \
	( (_list_)->list->data + ( (_index_) * (_list_)->list->elementSize )) \
)

#endif //SEALIB_LISTOF_H
