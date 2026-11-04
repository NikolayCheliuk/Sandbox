/**
 * Stack.c
 */

#include "Stack.h"

#include <stdio.h>

/**
 * Helper Functions
 */

/**
 * Lifecycle
 */

NCLResultType Stack_Init(StackType * Stack, size_t ElementSize)
{
  NCLResultType Result = NCL_E_OK;

  if (Stack == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = Vector_Init(&Stack->Vector, ElementSize);
  }

  return Result;
}

NCLResultType Stack_Deinit(StackType * Stack)
{
  NCLResultType Result = NCL_E_OK;

  if (Stack == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = Vector_Deinit(&Stack->Vector);
  }

  return Result;
}

/**
 * State Operations
 */

NCLResultType Stack_Clear(StackType * Stack)
{
  NCLResultType Result = NCL_E_OK;

  if (Stack == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = Vector_Clear(&Stack->Vector);
  }

  return Result;
}

size_t Stack_Size(const StackType * Stack)
{
  size_t Result = 0U;

  if (Stack == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = Vector_Size(&Stack->Vector);
  }

  return Result;
}

bool Stack_IsEmpty(const StackType * Stack)
{
  bool Result = true;

  if (Stack == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = Vector_IsEmpty(&Stack->Vector);
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

const void * Stack_TopConst(const StackType * Stack)
{
  const void * Result = NULL;

  if (Stack == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = Vector_BackConst(&Stack->Vector);
  }

  return Result;
}

void * Stack_Top(StackType * Stack)
{
  void * Result = NULL;

  if (Stack == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = Vector_Back(&Stack->Vector);
  }

  return Result;
}

NCLResultType Stack_Push(StackType * Stack, const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Stack == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = Vector_PushBack(&Stack->Vector, Value);
  }

  return Result;
}

NCLResultType Stack_Pop(StackType * Stack)
{
  NCLResultType Result = NCL_E_OK;

  if (Stack == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = Vector_PopBack(&Stack->Vector);
  }

  return Result;
}

/**
 * Debug
 */

NCLResultType Stack_Print(const StackType * Stack, PrintDataFunctionType PrintData)
{
  NCLResultType Result = NCL_E_OK;

  if (Stack == NULL || PrintData == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = Vector_Print(&Stack->Vector, PrintData);
  }

  return Result;
}
