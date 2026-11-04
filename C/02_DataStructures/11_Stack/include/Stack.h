/**
 * Stack.h
 *
 * Notes:
 * - If Init fails, the stack is not considered initialized.
 *   Deinit must only be called on an initialized stack.
 * - After Deinit, only Init may be called.
 * - Stack_Push requires Value not to point into the stack's own storage.
 * - Pointers returned by Stack_Top and Stack_TopConst may be invalidated by Stack_Push.
 * - Print function is to be used for debug. Thus it may output hidden elements.
 */

#ifndef STACK_H
#define STACK_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "Vector.h"
#include "Utils.h"

typedef struct
{
  VectorType Vector;
} StackType;

/**
 * Lifecycle
 */

NCLResultType Stack_Init(StackType * Stack, size_t ElementSize);
NCLResultType Stack_Deinit(StackType * Stack);

/**
 * State Operations
 */

NCLResultType Stack_Clear(StackType * Stack);

size_t Stack_Size(const StackType * Stack);
bool Stack_IsEmpty(const StackType * Stack);

/**
 * Iterators
 */

/**
 * Access
 */

/**
 * Modification
 */

/**
 * Edge Operations
 */

const void * Stack_TopConst(const StackType * Stack);
void * Stack_Top(StackType * Stack);

NCLResultType Stack_Push(StackType * Stack, const void * Value);
NCLResultType Stack_Pop(StackType * Stack);

/**
 * Debug
 */

NCLResultType Stack_Print(const StackType * Stack, PrintDataFunctionType PrintData);

#endif // STACK_H
