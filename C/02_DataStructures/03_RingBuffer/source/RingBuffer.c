/**
 * RingBuffer.c
 */

#include "RingBuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Helper Functions
 */

static const void * RingBuffer_AddrAtConst(const RingBufferType * RingBuffer, size_t Index)
{
  const void * Addr = NULL;

  if (RingBuffer == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (RingBuffer->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (RingBuffer->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Index >= RingBuffer->Capacity)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Addr = RingBuffer->Data + RingBuffer->ElementSize * Index;
  }

  return Addr;
}

static void * RingBuffer_AddrAt(RingBufferType * RingBuffer, size_t Index)
{
  void * Addr = NULL;

  if (RingBuffer == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (RingBuffer->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (RingBuffer->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Index >= RingBuffer->Capacity)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Addr = RingBuffer->Data + RingBuffer->ElementSize * Index;
  }

  return Addr;
}

/**
 * Lifecycle
 */

NCLResultType RingBuffer_Init(RingBufferType * RingBuffer, size_t ElementSize, size_t Capacity)
{
  NCLResultType Result = NCL_E_OK;
  void * p = NULL;

  if (RingBuffer == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (Capacity == 0U || Capacity > SIZE_MAX / ElementSize)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    p = calloc(Capacity, ElementSize);

    if (p == NULL)
    {
      RingBuffer->Data = NULL;
      RingBuffer->ElementSize = 0U;
      RingBuffer->Capacity = 0U;
      RingBuffer->Size = 0U;
      RingBuffer->Head = 0U;
      RingBuffer->Tail = 0U;

      Result = NCL_E_ALLOC_FAILED;
    }
    else
    {
      RingBuffer->Data = p;
      RingBuffer->ElementSize = ElementSize;
      RingBuffer->Capacity = Capacity;
      RingBuffer->Size = 0U;
      RingBuffer->Head = 0U;
      RingBuffer->Tail = 0U;
    }
  }

  return Result;
}

NCLResultType RingBuffer_Deinit(RingBufferType * RingBuffer)
{
  NCLResultType Result = NCL_E_OK;

  if (RingBuffer == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    if (RingBuffer->Data != NULL)
    {
      free(RingBuffer->Data);
    }

    RingBuffer->Data = NULL;
    RingBuffer->ElementSize = 0U;
    RingBuffer->Capacity = 0U;
    RingBuffer->Size = 0U;
    RingBuffer->Head = 0U;
    RingBuffer->Tail = 0U;
  }

  return Result;
}

/**
 * State Operations
 */

NCLResultType RingBuffer_Clear(RingBufferType * RingBuffer)
{
  NCLResultType Result = NCL_E_OK;

  if (RingBuffer == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    RingBuffer->Head = 0U;
    RingBuffer->Tail = 0U;
    RingBuffer->Size = 0U;
  }

  return Result;
}

size_t RingBuffer_Size(const RingBufferType * RingBuffer)
{
  size_t Result = 0U;

  if (RingBuffer == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = RingBuffer->Size;
  }

  return Result;
}

bool RingBuffer_IsEmpty(const RingBufferType * RingBuffer)
{
  bool Result = true;

  if (RingBuffer == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = (RingBuffer->Size == 0U);
  }

  return Result;
}

bool RingBuffer_IsFull(const RingBufferType * RingBuffer)
{
  bool Result = false;

  if (RingBuffer == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = (RingBuffer->Size == RingBuffer->Capacity);
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

const void * RingBuffer_FrontConst(const RingBufferType * RingBuffer)
{
  const void * Front = NULL;

  if (RingBuffer == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (RingBuffer->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (RingBuffer->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (RingBuffer_IsEmpty(RingBuffer))
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Front = RingBuffer_AddrAtConst(RingBuffer, RingBuffer->Head);
  }

  return Front;
}

void * RingBuffer_Front(RingBufferType * RingBuffer)
{
  void * Front = NULL;

  if (RingBuffer == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (RingBuffer->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (RingBuffer->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (RingBuffer_IsEmpty(RingBuffer))
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Front = RingBuffer_AddrAt(RingBuffer, RingBuffer->Head);
  }

  return Front;
}

NCLResultType RingBuffer_Push(RingBufferType * RingBuffer, const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (RingBuffer == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (RingBuffer->ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (RingBuffer->Data == NULL)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (RingBuffer_IsFull(RingBuffer))
  {
    Result = NCL_E_FULL;
  }
  else
  {
    memcpy(RingBuffer_AddrAt(RingBuffer, RingBuffer->Tail), Value, RingBuffer->ElementSize);

    RingBuffer->Tail = (RingBuffer->Tail + 1U) % RingBuffer->Capacity;
    // RingBuffer->Tail = (RingBuffer->Tail == RingBuffer->Capacity - 1U) ? 0U : RingBuffer->Tail + 1U;  // Safe

    RingBuffer->Size++;
  }

  return Result;
}

NCLResultType RingBuffer_Pop(RingBufferType * RingBuffer)
{
  NCLResultType Result = NCL_E_OK;

  if (RingBuffer == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (RingBuffer->ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (RingBuffer->Data == NULL)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (RingBuffer_IsEmpty(RingBuffer))
  {
    Result = NCL_E_EMPTY;
  }
  else
  {
    RingBuffer->Head = (RingBuffer->Head + 1U) % RingBuffer->Capacity;
    // RingBuffer->Head = (RingBuffer->Head == RingBuffer->Capacity - 1U) ? 0U : RingBuffer->Head + 1U;  // Safe

    RingBuffer->Size--;
  }

  return Result;
}

/**
 * Debug
 */

NCLResultType RingBuffer_Print(const RingBufferType * RingBuffer, PrintDataFunctionType PrintData)
{
  NCLResultType Result = NCL_E_OK;

  if (RingBuffer == NULL || PrintData == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (RingBuffer->ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (RingBuffer->Data == NULL)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    printf("RingBuffer Data: \n");

    for (size_t i = 0; i < RingBuffer->Capacity; i++)
    {
      PrintData(&RingBuffer->Data[RingBuffer->ElementSize * i]);
    }

    printf("\n");

    printf("RingBuffer->ElementSize : %zu\n", RingBuffer->ElementSize);
    printf("RingBuffer->Capacity    : %zu\n", RingBuffer->Capacity);
    printf("RingBuffer->Size        : %zu\n", RingBuffer->Size);
    printf("RingBuffer->Head        : %zu\n", RingBuffer->Head);
    printf("RingBuffer->Tail        : %zu\n", RingBuffer->Tail);

    printf("\n");
  }

  return Result;
}
