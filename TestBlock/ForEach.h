#ifndef SEALIB_FOREACH_H
#define SEALIB_FOREACH_H

#include "Macro.h"

#define ___forEachImpl(list, type, name_node) \
    var name_node = (list)->head;          \
    for ( \
        struct { type value; size_t index; } __it = { (type)(list)->head->data, 0 }; \
        name_node != NULL; \
        name_node = name_node->next, name_node != NULL && (__it.value = (type)name_node->data) && (__it.index = __it.index+1 ) \
    )

#define forEach(list) ___forEachImpl(list, typeOf((list)->payload), SEA_PP_CONCAT(__foreach_node, __COUNTER__) )
#define forEachValue (*__it.value)
#define forEachIndex (__it.index)

#endif //SEALIB_FOREACH_H
