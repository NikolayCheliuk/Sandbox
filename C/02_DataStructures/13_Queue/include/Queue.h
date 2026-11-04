/**
 * Queue.h
 *
 * Notes:
 * - If Init fails, the queue is not considered initialized.
 *   Deinit must only be called on an initialized queue.
 * - After Deinit, only Init may be called.
 * - Print function is to be used for debug. Thus it may output hidden elements.
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "RingBuffer.h"
#include "Utils.h"

typedef struct
{
  RingBufferType RingBuffer;
} QueueType;

/**
 * Lifecycle
 */

NCLResultType Queue_Init(QueueType * Queue, size_t ElementSize, size_t Capacity);
NCLResultType Queue_Deinit(QueueType * Queue);

/**
 * State Operations
 */

NCLResultType Queue_Clear(QueueType * Queue);

size_t Queue_Size(const QueueType * Queue);
bool Queue_IsEmpty(const QueueType * Queue);
bool Queue_IsFull(const QueueType * Queue);

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

const void * Queue_FrontConst(const QueueType * Queue);
void * Queue_Front(QueueType * Queue);

NCLResultType Queue_Push(QueueType * Queue, const void * Value);
NCLResultType Queue_Pop(QueueType * Queue);

/**
 * Debug
 */

NCLResultType Queue_Print(const QueueType * Queue, PrintDataFunctionType PrintData);

#endif // QUEUE_H
