#ifndef SEALIB_MEMORY_H
#define SEALIB_MEMORY_H

#define SEA_MALLOC_SCOPE(size) \
    for (uint8_t *SEA_SCOPE_BUFFER = malloc(size); SEA_SCOPE_BUFFER != NULL; free(SEA_SCOPE_BUFFER), SEA_SCOPE_BUFFER = NULL) \

#define SEA_REFCOUNT(info)

#endif //SEALIB_MEMORY_H