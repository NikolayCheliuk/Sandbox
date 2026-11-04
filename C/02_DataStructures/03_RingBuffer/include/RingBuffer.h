/**
 * RingBuffer.h
 *
 * Notes:
 * - If Init fails, the ring buffer is not considered initialized.
 *   Deinit must only be called on an initialized ring buffer.
 * - After Deinit, only Init may be called.
 * - Push requires Value to point outside the ring buffer's storage.
 * - Print function is to be used for debug. Thus it may output hidden elements.
 *
 * - | Operation              | Index   |
 *   | RingBuffer_Front	      | Head    |
 *   | RingBuffer_Push	      | Tail    |
 *   | RingBuffer_Pop	        | Head    |
 */

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

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
  size_t Head;          /* Read index */
  size_t Tail;          /* Write index */
} RingBufferType;

/**
 * Lifecycle
 */

NCLResultType RingBuffer_Init(RingBufferType * RingBuffer, size_t ElementSize, size_t Capacity);
NCLResultType RingBuffer_Deinit(RingBufferType * RingBuffer);

/**
 * State Operations
 */

NCLResultType RingBuffer_Clear(RingBufferType * RingBuffer);

size_t RingBuffer_Size(const RingBufferType * RingBuffer);
bool RingBuffer_IsEmpty(const RingBufferType * RingBuffer);
bool RingBuffer_IsFull(const RingBufferType * RingBuffer);

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

const void * RingBuffer_FrontConst(const RingBufferType * RingBuffer);
void * RingBuffer_Front(RingBufferType * RingBuffer);

NCLResultType RingBuffer_Push(RingBufferType * RingBuffer, const void * Value);
NCLResultType RingBuffer_Pop(RingBufferType * RingBuffer);

/**
 * Debug
 */

NCLResultType RingBuffer_Print(const RingBufferType * RingBuffer, PrintDataFunctionType PrintData);

#endif // RINGBUFFER_H
