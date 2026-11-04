/**
 * Deque.h
 *
 * Notes:
 * - If Init fails, the deque is not considered initialized.
 *   Deinit must only be called on an initialized deque.
 * - After Deinit, only Init may be called.
 * - DequeType must not be copied using assignment or memcpy.
 * - Print function is to be used for debug. Thus it may output hidden elements.
 */

#ifndef DEQUE_H
#define DEQUE_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "List.h"
#include "Utils.h"

typedef struct
{
  ListType List;
} DequeType;

/**
 * Lifecycle
 */

NCLResultType Deque_Init(DequeType * Deque, size_t ElementSize);
NCLResultType Deque_Deinit(DequeType * Deque);

/**
 * State Operations
 */

NCLResultType Deque_Clear(DequeType * Deque);

size_t Deque_Size(const DequeType * Deque);
bool Deque_IsEmpty(const DequeType * Deque);

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

const void * Deque_FrontConst(const DequeType * Deque);
void * Deque_Front(DequeType * Deque);

const void * Deque_BackConst(const DequeType * Deque);
void * Deque_Back(DequeType * Deque);

NCLResultType Deque_PushFront(DequeType * Deque, const void * Value);
NCLResultType Deque_PushBack(DequeType * Deque, const void * Value);

NCLResultType Deque_PopFront(DequeType * Deque);
NCLResultType Deque_PopBack(DequeType * Deque);

/**
 * Debug
 */

NCLResultType Deque_Print(const DequeType * Deque, PrintDataFunctionType PrintData);

#endif // DEQUE_H
