/**
 * Queue.c
 */

#include "Queue.h"

#include <stdio.h>

/**
 * Helper Functions
 */

/**
 * Lifecycle
 */

NCLResultType Queue_Init(QueueType * Queue, size_t ElementSize, size_t Capacity)
{
  NCLResultType Result = NCL_E_OK;

  if (Queue == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = RingBuffer_Init(&Queue->RingBuffer, ElementSize, Capacity);
  }

  return Result;
}

NCLResultType Queue_Deinit(QueueType * Queue)
{
  NCLResultType Result = NCL_E_OK;

  if (Queue == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = RingBuffer_Deinit(&Queue->RingBuffer);
  }

  return Result;
}

/**
 * State Operations
 */

NCLResultType Queue_Clear(QueueType * Queue)
{
  NCLResultType Result = NCL_E_OK;

  if (Queue == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = RingBuffer_Clear(&Queue->RingBuffer);
  }

  return Result;
}

size_t Queue_Size(const QueueType * Queue)
{
  size_t Result = 0U;

  if (Queue == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = RingBuffer_Size(&Queue->RingBuffer);
  }

  return Result;
}

bool Queue_IsEmpty(const QueueType * Queue)
{
  bool Result = true;

  if (Queue == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = RingBuffer_IsEmpty(&Queue->RingBuffer);
  }

  return Result;
}

bool Queue_IsFull(const QueueType * Queue)
{
  bool Result = false;

  if (Queue == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = RingBuffer_IsFull(&Queue->RingBuffer);
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

const void * Queue_FrontConst(const QueueType * Queue)
{
  const void * Result = NULL;

  if (Queue == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = RingBuffer_FrontConst(&Queue->RingBuffer);
  }

  return Result;
}

void * Queue_Front(QueueType * Queue)
{
  void * Result = NULL;

  if (Queue == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = RingBuffer_Front(&Queue->RingBuffer);
  }

  return Result;
}

NCLResultType Queue_Push(QueueType * Queue, const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Queue == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = RingBuffer_Push(&Queue->RingBuffer, Value);
  }

  return Result;
}

NCLResultType Queue_Pop(QueueType * Queue)
{
  NCLResultType Result = NCL_E_OK;

  if (Queue == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = RingBuffer_Pop(&Queue->RingBuffer);
  }

  return Result;
}

/**
 * Debug
 */

NCLResultType Queue_Print(const QueueType * Queue, PrintDataFunctionType PrintData)
{
  NCLResultType Result = NCL_E_OK;

  if (Queue == NULL || PrintData == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = RingBuffer_Print(&Queue->RingBuffer, PrintData);
  }

  return Result;
}
