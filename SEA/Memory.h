#ifndef SEA_MEMORY_H
#define SEA_MEMORY_H

// =================================
// MARK: Scope
// =================================

#define SEA_MALLOC_SCOPE(size) \
    for (uint8_t *SEA_SCOPE_BUFFER = malloc(size); SEA_SCOPE_BUFFER != NULL; free(SEA_SCOPE_BUFFER), SEA_SCOPE_BUFFER = NULL) \

#endif //SEA_MEMORY_H