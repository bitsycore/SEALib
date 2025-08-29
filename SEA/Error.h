#ifndef SEALIB_ERROR_H
#define SEALIB_ERROR_H

// =======================================
// MARK: Enums
// =======================================

enum SEA_ErrorType {
	SEA_ERROR_NONE = 0,

	SEA_ERROR_GENERIC_SELF_NULL = -1001,
	SEA_ERROR_GENERIC_ARGUMENT_NULL = -1002,

	SEA_ERROR_ARENA_OUT_OF_MEMORY = -2001,
	SEA_ERROR_ARENA_INVALID_ALIGNMENT = -2002,
	SEA_ERROR_ARENA_INVALID_CONTEXT = -2003,

	SEA_ERROR_ALLOCATOR_GENERIC_ERROR = -3000,
	SEA_ERROR_ALLOCATOR_ALLOC_FUNC_NULL = -3001,
	SEA_ERROR_ALLOCATOR_FREE_FUNC_NULL = -3002,
	SEA_ERROR_ALLOCATOR_INVALID_CONTEXT = -3003,

};

// =======================================
// MARK: Static
// =======================================

void SEA_Error_SetError(enum SEA_ErrorType error);
enum SEA_ErrorType SEA_Error_GetError();

#endif //SEALIB_ERROR_H