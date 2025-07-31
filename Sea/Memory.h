#ifndef SEALIB_MEMORY_H
#define SEALIB_MEMORY_H

#define SEA_MALLOC_SCOPE(size) \
    for (uint8_t *SCOPE_PTR = malloc(size); SCOPE_PTR != NULL; free(SCOPE_PTR), SCOPE_PTR = NULL) \

#define SEA_REFCOUNT(info)

#endif //SEALIB_MEMORY_H