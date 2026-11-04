/**
 * Deque.c
 */

#include "Deque.h"

#include <stdio.h>

/**
 * Helper Functions
 */

/**
 * Lifecycle
 */

NCLResultType Deque_Init(DequeType * Deque, size_t ElementSize)
{
  NCLResultType Result = NCL_E_OK;

  if (Deque == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = List_Init(&Deque->List, ElementSize);
  }

  return Result;
}

NCLResultType Deque_Deinit(DequeType * Deque)
{
  NCLResultType Result = NCL_E_OK;

  if (Deque == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = List_Deinit(&Deque->List);
  }

  return Result;
}

/**
 * State Operations
 */

NCLResultType Deque_Clear(DequeType * Deque)
{
  NCLResultType Result = NCL_E_OK;

  if (Deque == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = List_Clear(&Deque->List);
  }

  return Result;
}

size_t Deque_Size(const DequeType * Deque)
{
  size_t Result = 0U;

  if (Deque == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = List_Size(&Deque->List);
  }

  return Result;
}

bool Deque_IsEmpty(const DequeType * Deque)
{
  bool Result = true;

  if (Deque == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = List_IsEmpty(&Deque->List);
  }

  return Result;
}

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

const void * Deque_FrontConst(const DequeType * Deque)
{
  const void * Result = NULL;

  if (Deque == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = List_FrontConst(&Deque->List);
  }

  return Result;
}

void * Deque_Front(DequeType * Deque)
{
  void * Result = NULL;

  if (Deque == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = List_Front(&Deque->List);
  }

  return Result;
}

const void * Deque_BackConst(const DequeType * Deque)
{
  const void * Result = NULL;

  if (Deque == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = List_BackConst(&Deque->List);
  }

  return Result;
}

void * Deque_Back(DequeType * Deque)
{
  void * Result = NULL;

  if (Deque == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = List_Back(&Deque->List);
  }

  return Result;
}

NCLResultType Deque_PushFront(DequeType * Deque, const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Deque == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = List_PushFront(&Deque->List, Value);
  }

  return Result;
}

NCLResultType Deque_PushBack(DequeType * Deque, const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Deque == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = List_PushBack(&Deque->List, Value);
  }

  return Result;
}

NCLResultType Deque_PopFront(DequeType * Deque)
{
  NCLResultType Result = NCL_E_OK;

  if (Deque == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = List_PopFront(&Deque->List);
  }

  return Result;
}

NCLResultType Deque_PopBack(DequeType * Deque)
{
  NCLResultType Result = NCL_E_OK;

  if (Deque == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = List_PopBack(&Deque->List);
  }

  return Result;
}

/**
 * Debug
 */

NCLResultType Deque_Print(const DequeType * Deque, PrintDataFunctionType PrintData)
{
  NCLResultType Result = NCL_E_OK;

  if (Deque == NULL || PrintData == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = List_Print(&Deque->List, PrintData);
  }

  return Result;
}
