/**
 * Heap.c
 */

#include "Heap.h"

#include <stdio.h>
#include <string.h>

/**
 * Helper Functions
 */

static size_t Heap_ParentIndex(size_t ChildIndex)
{
  return (ChildIndex - 1U) / 2U;
}

static size_t Heap_LeftIndex(size_t ParentIndex)
{
  return ParentIndex * 2U + 1U;
}

static size_t Heap_RightIndex(size_t ParentIndex)
{
  return ParentIndex * 2U + 2U;
}

static void Heap_SiftUp(HeapType * Heap, size_t ChildIndex)
{
  size_t ParentIndex = 0U;

  uint8_t * ChildValue = NULL;
  uint8_t * ParentValue = NULL;

  while (ChildIndex != 0)
  {
    ParentIndex = Heap_ParentIndex(ChildIndex);

    ChildValue  = Vector_DataAt(&Heap->Vector, ChildIndex);
    ParentValue = Vector_DataAt(&Heap->Vector, ParentIndex);

    if (Heap->OrderFunction(ParentValue, ChildValue))
    {
      Swap(ParentValue, ChildValue, Heap->Vector.ElementSize);
      ChildIndex = ParentIndex;
    }
    else
    {
      break;
    }
  }
}

static void Heap_SiftDown(HeapType * Heap, size_t ParentIndex)
{
  size_t LeftIndex      = 0U;
  size_t RightIndex     = 0U;
  size_t ChildIndex     = 0U;

  uint8_t * LeftValue   = NULL;
  uint8_t * RightValue  = NULL;
  uint8_t * ChildValue  = NULL;
  uint8_t * ParentValue = NULL;

  // while (Heap_LeftIndex(ParentIndex) < Heap_Size(Heap))
  // Left child exists if 2 * ParentIndex + 1 < Size.
  // Rearranged to avoid size_t overflow when calculating the child index.
  while ((Heap_Size(Heap) >= 2U) && (ParentIndex <= (Heap_Size(Heap) - 2U) / 2U))
  {
    LeftIndex = Heap_LeftIndex(ParentIndex);
    LeftValue = Vector_DataAt(&Heap->Vector, LeftIndex);

    // if (Heap_RightIndex(ParentIndex) < Heap_Size(Heap))
    // Right child exists if 2 * ParentIndex + 2 < Size.
    // Rearranged to avoid size_t overflow when calculating the child index.
    if ((Heap_Size(Heap) >= 3U) && (ParentIndex <= (Heap_Size(Heap) - 3U) / 2U))
    {
      RightIndex = Heap_RightIndex(ParentIndex);
      RightValue = Vector_DataAt(&Heap->Vector, RightIndex);

      if (Heap->OrderFunction(RightValue, LeftValue))
      {
        ChildIndex = LeftIndex;
        ChildValue = LeftValue;
      }
      else
      {
        ChildIndex = RightIndex;
        ChildValue = RightValue;
      }
    }
    else
    {
      ChildIndex = LeftIndex;
      ChildValue = LeftValue;
    }

    ParentValue = Vector_DataAt(&Heap->Vector, ParentIndex);

    if (Heap->OrderFunction(ParentValue, ChildValue))
    {
      Swap(ParentValue, ChildValue, Heap->Vector.ElementSize);
      ParentIndex = ChildIndex;
    }
    else
    {
      break;
    }
  }
}

/**
 * Lifecycle
 */

NCLResultType Heap_Init(HeapType * Heap, size_t ElementSize, OrderFunctionType OrderFunction)
{
  NCLResultType Result = NCL_E_OK;

  if (Heap == NULL || OrderFunction == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = Vector_Init(&Heap->Vector, ElementSize);

    if (Result == NCL_E_OK)
    {
      Heap->OrderFunction = OrderFunction;
    }
  }

  return Result;
}

NCLResultType Heap_Deinit(HeapType * Heap)
{
  NCLResultType Result = NCL_E_OK;

  if (Heap == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = Vector_Deinit(&Heap->Vector);

    Heap->OrderFunction = NULL;
  }

  return Result;
}

/**
 * State Operations
 */

NCLResultType Heap_Clear(HeapType * Heap)
{
  NCLResultType Result = NCL_E_OK;

  if (Heap == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = Vector_Clear(&Heap->Vector);
  }

  return Result;
}

size_t Heap_Size(const HeapType * Heap)
{
  size_t Result = 0U;

  if (Heap == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = Vector_Size(&Heap->Vector);
  }

  return Result;
}

bool Heap_IsEmpty(const HeapType * Heap)
{
  bool Result = true;

  if (Heap == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = Vector_IsEmpty(&Heap->Vector);
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

const void * Heap_Top(const HeapType * Heap)
{
  const void * Top = NULL;

  if (Heap == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (Heap_IsEmpty(Heap))
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Top = Vector_FrontConst(&Heap->Vector);
  }

  return Top;
}

NCLResultType Heap_Push(HeapType * Heap, const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Heap == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = Vector_PushBack(&Heap->Vector, Value);

    if (Result == NCL_E_OK)
    {
      Heap_SiftUp(Heap, Vector_Size(&Heap->Vector) - 1U);
    }
  }

  return Result;
}

NCLResultType Heap_Pop(HeapType * Heap)
{
  NCLResultType Result = NCL_E_OK;

  if (Heap == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (Heap->Vector.ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (Heap_IsEmpty(Heap))
  {
    Result = NCL_E_EMPTY;
  }
  else if (Vector_Size(&Heap->Vector) == 1U)
  {
    Result = Vector_PopBack(&Heap->Vector);
  }
  else
  {
    Swap(Vector_DataAt(&Heap->Vector, 0U), Vector_DataAt(&Heap->Vector, (Vector_Size(&Heap->Vector) - 1U)), Heap->Vector.ElementSize);

    Result = Vector_PopBack(&Heap->Vector);

    if (Result == NCL_E_OK)
    {
      Heap_SiftDown(Heap, 0U);
    }
  }

  return Result;
}

/**
 * Debug
 */

NCLResultType Heap_Print(const HeapType * Heap, PrintDataFunctionType PrintData)
{
  NCLResultType Result = NCL_E_OK;

  if (Heap == NULL || PrintData == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = Vector_Print(&Heap->Vector, PrintData);
  }

  return Result;
}
