#ifndef SEA_SCOPE_UTILS_H
#define SEA_SCOPE_UTILS_H

#define SEA_SCOPE_MALLOC(size) \
    for (uint8_t *SEA_SCOPE_BUFFER = malloc(size); SEA_SCOPE_BUFFER != NULL; free(SEA_SCOPE_BUFFER), SEA_SCOPE_BUFFER = NULL) \

#define SEA_SCOPE_ARENA(arena) \
    for (bool _done = false; _done == false; _done = true, SEA_Arena.reset(arena))

#endif //SEA_SCOPE_UTILS_H