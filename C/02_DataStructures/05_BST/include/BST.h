/**
 * BST.h
 *
 * Notes:
 * - If Init fails, the BST is not considered initialized.
 *   Deinit must only be called on an initialized BST.
 * - After Deinit, only Init may be called.
 * - Print function is to be used for debug.
 */

#ifndef BST_H
#define BST_H

#include <inttypes.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>

#include "Utils.h"

typedef struct BSTNode
{
  struct BSTNode * Parent;
  struct BSTNode * Left;
  struct BSTNode * Right;
  _Alignas(long double) uint8_t Data[];
} BSTNodeType;

typedef struct
{
  BSTNodeType * Root;
  size_t ElementSize;
  size_t Size;
  CompareFunctionType CompareFunction;
} BSTType;

typedef struct
{
  BSTType * BST;
  BSTNodeType * Node;
} BSTIteratorType;

typedef struct
{
  const BSTType * BST;
  const BSTNodeType * Node;
} ConstBSTIteratorType;

/**
 * Lifecycle
 */

NCLResultType BST_Init(BSTType * BST, size_t ElementSize, CompareFunctionType CompareFunction);
NCLResultType BST_Deinit(BSTType * BST);

/**
 * State Operations
 */

NCLResultType BST_Clear(BSTType * BST);

size_t BST_Size(const BSTType * BST);
bool BST_IsEmpty(const BSTType * BST);

/**
 * Iterators
 */

NCLResultType BSTIterator_InitConst(ConstBSTIteratorType * It);
NCLResultType BSTIterator_Init(BSTIteratorType * It);

ConstBSTIteratorType BSTIterator_NextConst(ConstBSTIteratorType It);
BSTIteratorType BSTIterator_Next(BSTIteratorType It);

ConstBSTIteratorType BSTIterator_PrevConst(ConstBSTIteratorType It);
BSTIteratorType BSTIterator_Prev(BSTIteratorType It);

const void * BSTIterator_DataConst(ConstBSTIteratorType It);
const void * BSTIterator_Data(BSTIteratorType It);

/**
 * Compares two const iterators.
 *
 * Returns true if both iterators refer to the same node
 * in the same BST.
 *
 * Returns false if either iterator is invalid.
 * An iterator is invalid if its BST is NULL.
 *
 * End iterators (Node == NULL) are valid for comparison.
 */
bool BSTIterator_EqualConst(ConstBSTIteratorType It1, ConstBSTIteratorType It2);

/**
 * Compares two iterators.
 *
 * Returns true if both iterators refer to the same node
 * in the same BST.
 *
 * Returns false if either iterator is invalid.
 * An iterator is invalid if its BST is NULL.
 *
 * End iterators (Node == NULL) are valid for comparison.
 */
bool BSTIterator_Equal(BSTIteratorType It1, BSTIteratorType It2);

ConstBSTIteratorType BST_BeginConst(const BSTType * BST);
BSTIteratorType BST_Begin(BSTType * BST);

ConstBSTIteratorType BST_EndConst(const BSTType * BST);
BSTIteratorType BST_End(BSTType * BST);

/**
 * Access
 */

bool BST_Contains(const BSTType * BST, const void * Value);

/**
 * Modification
 */

NCLResultType BST_Insert(BSTType * BST, const void * Value);
NCLResultType BST_Erase(BSTType * BST, const void * Value);

/**
 * Edge Operations
 */

/**
 * Debug
 */

NCLResultType BST_Print(const BSTType * BST, PrintDataFunctionType PrintData);

#endif // BST_H
