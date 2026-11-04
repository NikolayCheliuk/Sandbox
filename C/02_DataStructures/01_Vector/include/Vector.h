/**
 * Vector.h
 *
 * Notes:
 * - If Init fails, the vector is not considered initialized.
 *   Deinit must only be called on an initialized vector.
 * - After Deinit, only Init may be called.
 * - Resize zero-initializes newly added elements.
 * - Insert, PushFront and PushBack require Value to point outside the vector's storage.
 * - Iterators are index-based; insertion, deletion and resizing may change
 *   which element an iterator refers to, or whether it is valid.
 * - Pointers to elements may be invalidated by reallocation or modification.
 * - Print is intended for debugging and displays all Capacity slots.
 *   Slots beyond Size are not logical elements and may contain stale data.
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "Utils.h"

typedef struct
{
  uint8_t * Data;
  size_t ElementSize;
  size_t Capacity;
  size_t Size;
} VectorType;

typedef struct
{
  VectorType * Vector;
  size_t Index;
} VectorIteratorType;

typedef struct
{
  const VectorType * Vector;
  size_t Index;
} ConstVectorIteratorType;

/**
 * Lifecycle
 */

NCLResultType Vector_Init(VectorType * Vector, size_t ElementSize);
NCLResultType Vector_Deinit(VectorType * Vector);

/**
 * State Operations
 */

NCLResultType Vector_Reserve(VectorType * Vector, size_t Capacity);
NCLResultType Vector_Resize(VectorType * Vector, size_t Size);
NCLResultType Vector_Clear(VectorType * Vector);

size_t Vector_Size(const VectorType * Vector);
bool Vector_IsEmpty(const VectorType * Vector);

/**
 * Iterators
 */

NCLResultType VectorIterator_InitConst(ConstVectorIteratorType * It);
NCLResultType VectorIterator_Init(VectorIteratorType * It);

ConstVectorIteratorType VectorIterator_NextConst(ConstVectorIteratorType It);
VectorIteratorType VectorIterator_Next(VectorIteratorType It);

ConstVectorIteratorType VectorIterator_PrevConst(ConstVectorIteratorType It);
VectorIteratorType VectorIterator_Prev(VectorIteratorType It);

const void * VectorIterator_DataConst(ConstVectorIteratorType It);
void * VectorIterator_Data(VectorIteratorType It);

/**
 * Compares two constant iterators.
 *
 * Returns true if both iterators refer to the same position
 * in the same vector.
 *
 * Returns false if either iterator is invalid.
 * An iterator is invalid if its Vector is NULL or
 * its Index is greater than the vector's Size.
 *
 * End iterators (Index == Size) are valid for comparison.
 */
bool VectorIterator_EqualConst(ConstVectorIteratorType It1, ConstVectorIteratorType It2);

/**
 * Compares two iterators.
 *
 * Returns true if both iterators refer to the same position
 * in the same vector.
 *
 * Returns false if either iterator is invalid.
 * An iterator is invalid if its Vector is NULL or
 * its Index is greater than the vector's Size.
 *
 * End iterators (Index == Size) are valid for comparison.
 */
bool VectorIterator_Equal(VectorIteratorType It1, VectorIteratorType It2);

ConstVectorIteratorType Vector_BeginConst(const VectorType * Vector);
VectorIteratorType Vector_Begin(VectorType * Vector);

ConstVectorIteratorType Vector_EndConst(const VectorType * Vector);
VectorIteratorType Vector_End(VectorType * Vector);

/**
 * Access
 */

const void * Vector_DataAtConst(const VectorType * Vector, size_t Index);
void * Vector_DataAt(VectorType * Vector, size_t Index);

NCLResultType Vector_Get(const VectorType * Vector, size_t Index, void * Value);
NCLResultType Vector_Set(VectorType * Vector, size_t Index, const void * Value);

/**
 * Modification
 */

VectorIteratorType Vector_Insert(VectorType * Vector, VectorIteratorType It, const void * Value);
VectorIteratorType Vector_Erase(VectorType * Vector, VectorIteratorType It);

/**
 * Edge Operations
 */

const void * Vector_FrontConst(const VectorType * Vector);
void * Vector_Front(VectorType * Vector);

const void * Vector_BackConst(const VectorType * Vector);
void * Vector_Back(VectorType * Vector);

NCLResultType Vector_PushFront(VectorType * Vector, const void * Value);
NCLResultType Vector_PushBack(VectorType * Vector, const void * Value);

NCLResultType Vector_PopFront(VectorType * Vector);
NCLResultType Vector_PopBack(VectorType * Vector);

/**
 * Debug
 */

NCLResultType Vector_Print(const VectorType * Vector, PrintDataFunctionType PrintData);

#endif // VECTOR_H
