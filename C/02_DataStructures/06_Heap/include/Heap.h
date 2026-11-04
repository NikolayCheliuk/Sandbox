/**
 * Heap.h
 *
 * Notes:
 * - If Init fails, the heap is not considered initialized.
 *   Deinit must only be called on an initialized heap.
 * - After Deinit, only Init may be called.
 * - OrderFunction(A, B) returns true if A precedes B in the ordering.
 *   Less creates a Max Heap; Greater creates a Min Heap.
 * - Print function is to be used for debug. Thus it may output hidden elements.
 */

#ifndef HEAP_H
#define HEAP_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "Vector.h"
#include "Utils.h"

typedef struct
{
  VectorType Vector;
  OrderFunctionType OrderFunction;
} HeapType;

/**
 * Lifecycle
 */

NCLResultType Heap_Init(HeapType * Heap, size_t ElementSize, OrderFunctionType OrderFunction);
NCLResultType Heap_Deinit(HeapType * Heap);

/**
 * State Operations
 */

NCLResultType Heap_Clear(HeapType * Heap);

size_t Heap_Size(const HeapType * Heap);
bool Heap_IsEmpty(const HeapType * Heap);

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

const void * Heap_Top(const HeapType * Heap);

NCLResultType Heap_Push(HeapType * Heap, const void * Value);

NCLResultType Heap_Pop(HeapType * Heap);

/**
 * Debug
 */

NCLResultType Heap_Print(const HeapType * Heap, PrintDataFunctionType PrintData);

#endif // HEAP_H
