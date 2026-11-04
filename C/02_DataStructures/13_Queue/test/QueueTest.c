/**
 * QueueTest.c
 *
 * Regression tests for the public Queue API.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "Queue.h"
#include "TestUtils.h"

/**
 * Lifecycle
 */

static void Test_Queue_Init_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Queue_Init(NULL, sizeof(uint32_t), 3U) == NCL_E_NULL_PTR);
}

static void Test_Queue_Init_ZeroElementSize(TestContextType * Context)
{
  QueueType Queue = { 0 };

  TEST_CHECK(Queue_Init(&Queue, 0U, 3U) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Queue_Init_ZeroCapacity(TestContextType * Context)
{
  QueueType Queue = { 0 };

  TEST_CHECK(Queue_Init(&Queue, sizeof(uint32_t), 0U) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Queue_Init_Overflow(TestContextType * Context)
{
  QueueType Queue = { 0 };

  TEST_CHECK(Queue_Init(&Queue, sizeof(uint32_t), SIZE_MAX) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Queue_Init_Valid(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Queue.RingBuffer.Data != NULL);
  TEST_CHECK(Queue.RingBuffer.ElementSize == sizeof(uint32_t));
  TEST_CHECK(Queue.RingBuffer.Capacity == 3U);
  TEST_CHECK(Queue.RingBuffer.Size == 0U);
  TEST_CHECK(Queue.RingBuffer.Head == 0U);
  TEST_CHECK(Queue.RingBuffer.Tail == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Deinit_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Queue_Deinit(NULL) == NCL_E_NULL_PTR);
}

static void Test_Queue_Deinit_Empty(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;


  TEST_REQUIRE(Queue_Deinit(&Queue) == NCL_E_OK);
  Initialized = false;

  TEST_CHECK(Queue.RingBuffer.Data == NULL);
  TEST_CHECK(Queue.RingBuffer.ElementSize == 0U);
  TEST_CHECK(Queue.RingBuffer.Capacity == 0U);
  TEST_CHECK(Queue.RingBuffer.Size == 0U);
  TEST_CHECK(Queue.RingBuffer.Head == 0U);
  TEST_CHECK(Queue.RingBuffer.Tail == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Deinit_NonEmpty(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 9U;

  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);

  TEST_REQUIRE(Queue_Deinit(&Queue) == NCL_E_OK);
  Initialized = false;

  TEST_CHECK(Queue.RingBuffer.Data == NULL);
  TEST_CHECK(Queue.RingBuffer.ElementSize == 0U);
  TEST_CHECK(Queue.RingBuffer.Capacity == 0U);
  TEST_CHECK(Queue.RingBuffer.Size == 0U);
  TEST_CHECK(Queue.RingBuffer.Head == 0U);
  TEST_CHECK(Queue.RingBuffer.Tail == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

/**
 * State Operations
 */

static void Test_Queue_Clear_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Queue_Clear(NULL) == NCL_E_NULL_PTR);
}

static void Test_Queue_Clear_Empty(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Queue_Clear(&Queue) == NCL_E_OK);
  TEST_CHECK(Queue_Size(&Queue) == 0U);
  TEST_CHECK(Queue.RingBuffer.Capacity == 3U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Clear_NonEmpty(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 7U;

  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_CHECK(Queue_Clear(&Queue) == NCL_E_OK);
  TEST_CHECK(Queue_Size(&Queue) == 0U);
  TEST_CHECK(Queue.RingBuffer.Head == 0U);
  TEST_CHECK(Queue.RingBuffer.Tail == 0U);
  TEST_CHECK(Queue.RingBuffer.Capacity == 3U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Size_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Queue_Size(NULL) == 0U);
}

static void Test_Queue_Size_Empty(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Queue_Size(&Queue) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Size_NonEmpty(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 4U;

  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_CHECK(Queue_Size(&Queue) == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_IsEmpty_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Queue_IsEmpty(NULL));
}

static void Test_Queue_IsEmpty_Empty(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Queue_IsEmpty(&Queue));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_IsEmpty_NonEmpty(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 4U;

  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_CHECK(!Queue_IsEmpty(&Queue));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_IsFull_NullPtr(TestContextType * Context)
{
  TEST_CHECK(!Queue_IsFull(NULL));
}

static void Test_Queue_IsFull_Empty(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(!Queue_IsFull(&Queue));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_IsFull_Partial(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 4U;

  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_CHECK(!Queue_IsFull(&Queue));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_IsFull_Full(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 4U;

  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_CHECK(Queue_IsFull(&Queue));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

/**
 * Edge Operations
 */

static void Test_Queue_FrontConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Queue_FrontConst(NULL) == NULL);
}

static void Test_Queue_FrontConst_InvalidData(TestContextType * Context)
{
  QueueType Queue = { 0 };

  TEST_CHECK(Queue_FrontConst(&Queue) == NULL);
}

static void Test_Queue_FrontConst_InvalidElementSize(TestContextType * Context)
{
  QueueType Queue = { 0 };
  uint32_t Value = 3U;
  Queue.RingBuffer.Data = (uint8_t *)&Value;

  TEST_CHECK(Queue_FrontConst(&Queue) == NULL);
}

static void Test_Queue_FrontConst_Empty(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Queue_FrontConst(&Queue) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_FrontConst_Valid(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 42U;

  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_CHECK(Queue_FrontConst(&Queue) != NULL);
  TEST_CHECK(*(uint32_t *)Queue_FrontConst(&Queue) == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Front_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Queue_Front(NULL) == NULL);
}

static void Test_Queue_Front_InvalidData(TestContextType * Context)
{
  QueueType Queue = { 0 };

  TEST_CHECK(Queue_Front(&Queue) == NULL);
}

static void Test_Queue_Front_InvalidElementSize(TestContextType * Context)
{
  QueueType Queue = { 0 };
  uint32_t Value = 3U;
  Queue.RingBuffer.Data = (uint8_t *)&Value;

  TEST_CHECK(Queue_Front(&Queue) == NULL);
}

static void Test_Queue_Front_Empty(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Queue_Front(&Queue) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Front_Valid(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 42U;

  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_CHECK(Queue_Front(&Queue) != NULL);
  TEST_CHECK(*(uint32_t *)Queue_Front(&Queue) == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Push_NullQueue(TestContextType * Context)
{
  uint32_t Value = 4U;

  TEST_CHECK(Queue_Push(NULL, &Value) == NCL_E_NULL_PTR);
}

static void Test_Queue_Push_NullValue(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Queue_Push(&Queue, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Push_InvalidData(TestContextType * Context)
{
  QueueType Queue = { 0 };
  uint32_t Value = 4U;

  TEST_CHECK(Queue_Push(&Queue, &Value) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Queue_Push_InvalidElementSize(TestContextType * Context)
{
  QueueType Queue = { 0 };
  uint32_t Value = 4U;
  Queue.RingBuffer.Data = (uint8_t *)&Value;

  TEST_CHECK(Queue_Push(&Queue, &Value) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Queue_Push_Full(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 4U;

  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_CHECK(Queue_Push(&Queue, &Value) == NCL_E_FULL);
  TEST_CHECK(Queue_Size(&Queue) == 3U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Push_Valid(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 12U;

  TEST_CHECK(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_CHECK(Queue_Size(&Queue) == 1U);
  TEST_CHECK(*(uint32_t *)Queue_Front(&Queue) == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Pop_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Queue_Pop(NULL) == NCL_E_NULL_PTR);
}

static void Test_Queue_Pop_InvalidData(TestContextType * Context)
{
  QueueType Queue = { 0 };

  TEST_CHECK(Queue_Pop(&Queue) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Queue_Pop_InvalidElementSize(TestContextType * Context)
{
  QueueType Queue = { 0 };
  uint32_t Value = 4U;
  Queue.RingBuffer.Data = (uint8_t *)&Value;

  TEST_CHECK(Queue_Pop(&Queue) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Queue_Pop_Empty(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Queue_Pop(&Queue) == NCL_E_EMPTY);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Pop_Valid(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 7U;

  TEST_REQUIRE(Queue_Push(&Queue, &Value) == NCL_E_OK);
  TEST_CHECK(Queue_Pop(&Queue) == NCL_E_OK);
  TEST_CHECK(Queue_Size(&Queue) == 0U);
  TEST_CHECK(Queue_IsEmpty(&Queue));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_FIFO_Wraparound(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  uint32_t Values[] = { 10U, 20U, 30U, 40U, 50U };

  TEST_REQUIRE(Queue_Push(&Queue, &Values[0]) == NCL_E_OK);
  TEST_REQUIRE(Queue_Push(&Queue, &Values[1]) == NCL_E_OK);
  TEST_REQUIRE(Queue_Push(&Queue, &Values[2]) == NCL_E_OK);
  TEST_CHECK(*(uint32_t *)Queue_Front(&Queue) == 10U);
  TEST_REQUIRE(Queue_Pop(&Queue) == NCL_E_OK);
  TEST_REQUIRE(Queue_Pop(&Queue) == NCL_E_OK);
  TEST_REQUIRE(Queue_Push(&Queue, &Values[3]) == NCL_E_OK);
  TEST_REQUIRE(Queue_Push(&Queue, &Values[4]) == NCL_E_OK);
  TEST_CHECK(Queue_IsFull(&Queue));
  TEST_CHECK(*(uint32_t *)Queue_Front(&Queue) == 30U);
  TEST_REQUIRE(Queue_Pop(&Queue) == NCL_E_OK);
  TEST_CHECK(*(uint32_t *)Queue_Front(&Queue) == 40U);
  TEST_REQUIRE(Queue_Pop(&Queue) == NCL_E_OK);
  TEST_CHECK(*(uint32_t *)Queue_Front(&Queue) == 50U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

/**
 * Debug
 */

static NCLResultType Test_Queue_PrintData(const void * Value)
{
  (void)Value;
  return NCL_E_OK;
}

static void Test_Queue_Print_NullQueue(TestContextType * Context)
{
  TEST_CHECK(Queue_Print(NULL, Test_Queue_PrintData) == NCL_E_NULL_PTR);
}

static void Test_Queue_Print_NullCallback(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Queue_Print(&Queue, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Print_InvalidData(TestContextType * Context)
{
  QueueType Queue = { 0 };

  TEST_CHECK(Queue_Print(&Queue, Test_Queue_PrintData) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Queue_Print_InvalidElementSize(TestContextType * Context)
{
  QueueType Queue = { 0 };
  uint32_t Value = 4U;
  Queue.RingBuffer.Data = (uint8_t *)&Value;

  TEST_CHECK(Queue_Print(&Queue, Test_Queue_PrintData) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Queue_Print_Valid(TestContextType * Context)
{
  QueueType Queue = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Queue_Init(&Queue, sizeof(uint32_t), 3U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Queue_Print(&Queue, Test_Queue_PrintData) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Queue_Deinit(&Queue) == NCL_E_OK);
  }
}

static void Test_Queue_Lifecycle(TestContextType * Context)
{
  TEST_RUN(Test_Queue_Init_NullPtr);
  TEST_RUN(Test_Queue_Init_ZeroElementSize);
  TEST_RUN(Test_Queue_Init_ZeroCapacity);
  TEST_RUN(Test_Queue_Init_Overflow);
  TEST_RUN(Test_Queue_Init_Valid);
  TEST_RUN(Test_Queue_Deinit_NullPtr);
  TEST_RUN(Test_Queue_Deinit_Empty);
  TEST_RUN(Test_Queue_Deinit_NonEmpty);
}

static void Test_Queue_StateOperations(TestContextType * Context)
{
  TEST_RUN(Test_Queue_Clear_NullPtr);
  TEST_RUN(Test_Queue_Clear_Empty);
  TEST_RUN(Test_Queue_Clear_NonEmpty);
  TEST_RUN(Test_Queue_Size_NullPtr);
  TEST_RUN(Test_Queue_Size_Empty);
  TEST_RUN(Test_Queue_Size_NonEmpty);
  TEST_RUN(Test_Queue_IsEmpty_NullPtr);
  TEST_RUN(Test_Queue_IsEmpty_Empty);
  TEST_RUN(Test_Queue_IsEmpty_NonEmpty);
  TEST_RUN(Test_Queue_IsFull_NullPtr);
  TEST_RUN(Test_Queue_IsFull_Empty);
  TEST_RUN(Test_Queue_IsFull_Partial);
  TEST_RUN(Test_Queue_IsFull_Full);
}

static void Test_Queue_EdgeOperations(TestContextType * Context)
{
  TEST_RUN(Test_Queue_FrontConst_NullPtr);
  TEST_RUN(Test_Queue_FrontConst_InvalidData);
  TEST_RUN(Test_Queue_FrontConst_InvalidElementSize);
  TEST_RUN(Test_Queue_FrontConst_Empty);
  TEST_RUN(Test_Queue_FrontConst_Valid);
  TEST_RUN(Test_Queue_Front_NullPtr);
  TEST_RUN(Test_Queue_Front_InvalidData);
  TEST_RUN(Test_Queue_Front_InvalidElementSize);
  TEST_RUN(Test_Queue_Front_Empty);
  TEST_RUN(Test_Queue_Front_Valid);
  TEST_RUN(Test_Queue_Push_NullQueue);
  TEST_RUN(Test_Queue_Push_NullValue);
  TEST_RUN(Test_Queue_Push_InvalidData);
  TEST_RUN(Test_Queue_Push_InvalidElementSize);
  TEST_RUN(Test_Queue_Push_Full);
  TEST_RUN(Test_Queue_Push_Valid);
  TEST_RUN(Test_Queue_Pop_NullPtr);
  TEST_RUN(Test_Queue_Pop_InvalidData);
  TEST_RUN(Test_Queue_Pop_InvalidElementSize);
  TEST_RUN(Test_Queue_Pop_Empty);
  TEST_RUN(Test_Queue_Pop_Valid);
  TEST_RUN(Test_Queue_FIFO_Wraparound);
}

static void Test_Queue_Debug(TestContextType * Context)
{
  TEST_RUN(Test_Queue_Print_NullQueue);
  TEST_RUN(Test_Queue_Print_NullCallback);
  TEST_RUN(Test_Queue_Print_InvalidData);
  TEST_RUN(Test_Queue_Print_InvalidElementSize);
  TEST_RUN(Test_Queue_Print_Valid);
}

int main(void)
{
  TestContextType Context;

  TestUtils_Init(&Context);

  TestUtils_Run(&Context, Test_Queue_Lifecycle, "Queue Lifecycle");
  TestUtils_Run(&Context, Test_Queue_StateOperations, "Queue State Operations");
  TestUtils_Run(&Context, Test_Queue_EdgeOperations, "Queue Edge Operations");
  TestUtils_Run(&Context, Test_Queue_Debug, "Queue Debug");

  return TestUtils_Report(&Context);
}
