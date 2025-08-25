#ifndef SEALIB_FOREACH_H
#define SEALIB_FOREACH_H

#include "Macro.h"

#define ___forEachImpl(_list_, _type_) \
    for ( \
        struct { _type_ value; size_t index; } __it = { (_type_) ( (_list_)->data ), 0 }; \
        __it.index < (_list_)->size; \
        (__it.index+=1) && ( __it.value = (_type_) ( (_list_)->data + (__it.index * (_list_)->elementSize) ) ) \
    )

#define forEach(_list_) ___forEachImpl( (_list_)->list, typeOf((_list_)->payload) )
#define forEachValue (*__it.value)
#define forEachIndex (__it.index)

#endif //SEALIB_FOREACH_H
