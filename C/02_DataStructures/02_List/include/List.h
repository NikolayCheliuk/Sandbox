/**
 * List.h
 *
 * Notes:
 * - If Init fails, the list is not considered initialized.
 *   Deinit must only be called on an initialized list.
 * - After Deinit, only Init may be called.
 * - Index-based access is O(n).
 * - Pointers to elements and iterators remain valid until their elements are removed.
 * - The end iterator remains valid until Deinit.
 * - ListType must not be copied using assignment or memcpy.
 * - Print function is to be used for debug.
 */

#ifndef LIST_H
#define LIST_H

#include <inttypes.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>

#include "Utils.h"

typedef struct ListNode
{
  struct ListNode * Prev;
  struct ListNode * Next;
} ListNodeType;

typedef struct ListDataNode
{
  ListNodeType Base;
  _Alignas(long double) uint8_t Data[];
} ListDataNodeType;

typedef struct
{
  ListNodeType Sentinel;
  size_t ElementSize;
  size_t Size;
} ListType;

typedef struct
{
  ListType * List;
  ListNodeType * Node;
} ListIteratorType;

typedef struct
{
  const ListType * List;
  const ListNodeType * Node;
} ConstListIteratorType;

/**
 * Lifecycle
 */

NCLResultType List_Init(ListType * List, size_t ElementSize);
NCLResultType List_Deinit(ListType * List);

/**
 * State Operations
 */

NCLResultType List_Clear(ListType * List);

size_t List_Size(const ListType * List);
bool List_IsEmpty(const ListType * List);

/**
 * Iterators
 */

NCLResultType ListIterator_InitConst(ConstListIteratorType * It);
NCLResultType ListIterator_Init(ListIteratorType * It);

ConstListIteratorType ListIterator_NextConst(ConstListIteratorType It);
ListIteratorType ListIterator_Next(ListIteratorType It);

ConstListIteratorType ListIterator_PrevConst(ConstListIteratorType It);
ListIteratorType ListIterator_Prev(ListIteratorType It);

const void * ListIterator_DataConst(ConstListIteratorType It);
void * ListIterator_Data(ListIteratorType It);

/**
 * Compares two const iterators.
 *
 * Returns true if both iterators refer to the same node
 * in the same List.
 *
 * Returns false if either iterator is invalid.
 * An iterator is invalid if its List is NULL or its Node is NULL.
 *
 * End iterators (Node == &Sentinel) are valid for comparison.
 */
bool ListIterator_EqualConst(ConstListIteratorType It1, ConstListIteratorType It2);

/**
 * Compares two iterators.
 *
 * Returns true if both iterators refer to the same node
 * in the same List.
 *
 * Returns false if either iterator is invalid.
 * An iterator is invalid if its List is NULL or its Node is NULL.
 *
 * End iterators (Node == &Sentinel) are valid for comparison.
 */
bool ListIterator_Equal(ListIteratorType It1, ListIteratorType It2);

ConstListIteratorType List_BeginConst(const ListType * List);
ListIteratorType List_Begin(ListType * List);

ConstListIteratorType List_EndConst(const ListType * List);
ListIteratorType List_End(ListType * List);

/**
 * Access
 */

const void * List_DataAtConst(const ListType * List, size_t Index);
void * List_DataAt(ListType * List, size_t Index);

NCLResultType List_Get(const ListType * List, size_t Index, void * Value);
NCLResultType List_Set(ListType * List, size_t Index, const void * Value);

/**
 * Modification
 */

ListIteratorType List_Insert(ListType * List, ListIteratorType It, const void * Value);
ListIteratorType List_Erase(ListType * List, ListIteratorType It);

/**
 * Edge Operations
 */

const void * List_FrontConst(const ListType * List);
void * List_Front(ListType * List);

const void * List_BackConst(const ListType * List);
void * List_Back(ListType * List);

NCLResultType List_PushFront(ListType * List, const void * Value);
NCLResultType List_PushBack(ListType * List, const void * Value);

NCLResultType List_PopFront(ListType * List);
NCLResultType List_PopBack(ListType * List);

/**
 * Debug
 */

NCLResultType List_Print(const ListType * List, PrintDataFunctionType PrintData);

#endif // LIST_H
