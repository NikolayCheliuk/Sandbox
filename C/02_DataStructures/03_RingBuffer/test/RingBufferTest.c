/**
 * RingBufferTest.c
 *
 * Regression tests for the public RingBuffer API.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "RingBuffer.h"
#include "TestUtils.h"

static size_t PrintCount = 0U;

static NCLResultType PrintData(const void * Data)
{
  if (Data != NULL)
  {
    PrintCount++;
  }

  return NCL_E_OK;
}

/**
 * Lifecycle
 */

static void Test_RingBuffer_Init_NullPtr(TestContextType * Context)
{
  TEST_CHECK(RingBuffer_Init(NULL, sizeof(uint32_t), 4U) == NCL_E_NULL_PTR);
}

static void Test_RingBuffer_Init_ZeroElementSize(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };

  TEST_CHECK(RingBuffer_Init(&RingBuffer, 0U, 4U) == NCL_E_INVALID_ARGUMENT);
}

static void Test_RingBuffer_Init_ZeroCapacity(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };

  TEST_CHECK(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 0U) == NCL_E_INVALID_ARGUMENT);
}

static void Test_RingBuffer_Init_Overflow(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };

  TEST_CHECK(RingBuffer_Init(&RingBuffer, SIZE_MAX, 2U) == NCL_E_INVALID_ARGUMENT);
}

static void Test_RingBuffer_Init_Valid(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(RingBuffer.Data != NULL);
  TEST_CHECK(RingBuffer.ElementSize == sizeof(uint32_t));
  TEST_CHECK(RingBuffer.Capacity == 4U);
  TEST_CHECK(RingBuffer.Size == 0U);
  TEST_CHECK(RingBuffer.Head == 0U);
  TEST_CHECK(RingBuffer.Tail == 0U);

  for (size_t Index = 0U; Index < 4U; Index++)
  {
    TEST_CHECK(((uint32_t *)RingBuffer.Data)[Index] == 0U);
  }

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Deinit_NullPtr(TestContextType * Context)
{
  TEST_CHECK(RingBuffer_Deinit(NULL) == NCL_E_NULL_PTR);
}

static void Test_RingBuffer_Deinit_Allocated(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  Initialized = false;

  TEST_CHECK(RingBuffer.Data == NULL);
  TEST_CHECK(RingBuffer.ElementSize == 0U);
  TEST_CHECK(RingBuffer.Capacity == 0U);
  TEST_CHECK(RingBuffer.Size == 0U);
  TEST_CHECK(RingBuffer.Head == 0U);
  TEST_CHECK(RingBuffer.Tail == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Deinit_NullData(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };

  TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  TEST_CHECK(RingBuffer.Data == NULL);
  TEST_CHECK(RingBuffer.ElementSize == 0U);
  TEST_CHECK(RingBuffer.Capacity == 0U);
  TEST_CHECK(RingBuffer.Size == 0U);
  TEST_CHECK(RingBuffer.Head == 0U);
  TEST_CHECK(RingBuffer.Tail == 0U);
}

/**
 * State Operations
 */

static void Test_RingBuffer_Clear_NullPtr(TestContextType * Context)
{
  TEST_CHECK(RingBuffer_Clear(NULL) == NCL_E_NULL_PTR);
}

static void Test_RingBuffer_Clear_Empty(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(RingBuffer_Clear(&RingBuffer) == NCL_E_OK);
  TEST_CHECK(RingBuffer.Size == 0U);
  TEST_CHECK(RingBuffer.Head == 0U);
  TEST_CHECK(RingBuffer.Tail == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Clear_Wrapped(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 17U;

  for (size_t Index = 0U; Index < 4U; Index++)
  {
    TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_OK);
  }

  TEST_REQUIRE(RingBuffer_Pop(&RingBuffer) == NCL_E_OK);
  TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_OK);
  TEST_CHECK(RingBuffer.Head == 1U);
  TEST_CHECK(RingBuffer.Tail == 1U);
  TEST_CHECK(RingBuffer_Clear(&RingBuffer) == NCL_E_OK);
  TEST_CHECK(RingBuffer.Size == 0U);
  TEST_CHECK(RingBuffer.Head == 0U);
  TEST_CHECK(RingBuffer.Tail == 0U);
  TEST_CHECK(RingBuffer.Capacity == 4U);
  TEST_CHECK(RingBuffer.Data != NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Size_NullPtr(TestContextType * Context)
{
  TEST_CHECK(RingBuffer_Size(NULL) == 0U);
}

static void Test_RingBuffer_Size_Empty(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(RingBuffer_Size(&RingBuffer) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Size_NonEmpty(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 5U;

  TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_OK);
  TEST_CHECK(RingBuffer_Size(&RingBuffer) == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_IsEmpty_NullPtr(TestContextType * Context)
{
  TEST_CHECK(RingBuffer_IsEmpty(NULL));
}

static void Test_RingBuffer_IsEmpty_Empty(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(RingBuffer_IsEmpty(&RingBuffer));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_IsEmpty_NonEmpty(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 5U;

  TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_OK);
  TEST_CHECK(!RingBuffer_IsEmpty(&RingBuffer));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_IsFull_NullPtr(TestContextType * Context)
{
  TEST_CHECK(!RingBuffer_IsFull(NULL));
}

static void Test_RingBuffer_IsFull_NotFull(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(!RingBuffer_IsFull(&RingBuffer));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_IsFull_Full(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 5U;

  for (size_t Index = 0U; Index < 4U; Index++)
  {
    TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_OK);
  }

  TEST_CHECK(RingBuffer_IsFull(&RingBuffer));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

/**
 * Edge Operations
 */

static void Test_RingBuffer_FrontConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(RingBuffer_FrontConst(NULL) == NULL);
}

static void Test_RingBuffer_FrontConst_NullData(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };

  TEST_CHECK(RingBuffer_FrontConst(&RingBuffer) == NULL);
}

static void Test_RingBuffer_FrontConst_ZeroElementSize(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  uint32_t Value = 0U;
  RingBuffer.Data = (uint8_t *)&Value;

  TEST_CHECK(RingBuffer_FrontConst(&RingBuffer) == NULL);
}

static void Test_RingBuffer_FrontConst_Empty(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(RingBuffer_FrontConst(&RingBuffer) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_FrontConst_Valid(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 29U;

  TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_OK);
  TEST_CHECK(RingBuffer_FrontConst(&RingBuffer) == (void *)RingBuffer.Data);
  TEST_CHECK(*(const uint32_t *)RingBuffer_FrontConst(&RingBuffer) == 29U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_FrontConst_Wrapped(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t First = 11U;
  uint32_t Second = 22U;

  TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &First) == NCL_E_OK);
  TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Second) == NCL_E_OK);
  TEST_REQUIRE(RingBuffer_Pop(&RingBuffer) == NCL_E_OK);
  TEST_CHECK(RingBuffer_FrontConst(&RingBuffer) == (void *)(RingBuffer.Data + sizeof(uint32_t)));
  TEST_CHECK(*(const uint32_t *)RingBuffer_FrontConst(&RingBuffer) == 22U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Front_NullPtr(TestContextType * Context)
{
  TEST_CHECK(RingBuffer_Front(NULL) == NULL);
}

static void Test_RingBuffer_Front_NullData(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };

  TEST_CHECK(RingBuffer_Front(&RingBuffer) == NULL);
}

static void Test_RingBuffer_Front_ZeroElementSize(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  uint32_t Value = 0U;
  RingBuffer.Data = (uint8_t *)&Value;

  TEST_CHECK(RingBuffer_Front(&RingBuffer) == NULL);
}

static void Test_RingBuffer_Front_Empty(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(RingBuffer_Front(&RingBuffer) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Front_Valid(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 29U;

  TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_OK);
  TEST_CHECK(RingBuffer_Front(&RingBuffer) == (void *)RingBuffer.Data);
  TEST_CHECK(*(const uint32_t *)RingBuffer_Front(&RingBuffer) == 29U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Front_Wrapped(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t First = 11U;
  uint32_t Second = 22U;

  TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &First) == NCL_E_OK);
  TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Second) == NCL_E_OK);
  TEST_REQUIRE(RingBuffer_Pop(&RingBuffer) == NCL_E_OK);
  TEST_CHECK(RingBuffer_Front(&RingBuffer) == (void *)(RingBuffer.Data + sizeof(uint32_t)));
  TEST_CHECK(*(const uint32_t *)RingBuffer_Front(&RingBuffer) == 22U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Push_NullBuffer(TestContextType * Context)
{
  TEST_CHECK(RingBuffer_Push(NULL, &(uint32_t){ 5U }) == NCL_E_NULL_PTR);
}

static void Test_RingBuffer_Push_NullValue(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(RingBuffer_Push(&RingBuffer, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Push_InvalidData(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  uint32_t Value = 5U;

  TEST_CHECK(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_INVALID_ARGUMENT);
}

static void Test_RingBuffer_Push_ZeroElementSize(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  uint32_t Value = 5U;

  RingBuffer.Data = (uint8_t *)&Value;
  TEST_CHECK(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_INVALID_ARGUMENT);
}

static void Test_RingBuffer_Push_Full(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 5U;

  for (size_t Index = 0U; Index < 4U; Index++)
  {
    TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_OK);
  }

  TEST_CHECK(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_FULL);
  TEST_CHECK(RingBuffer.Size == 4U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Push_Valid(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 55U;

  TEST_CHECK(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_OK);
  TEST_CHECK(RingBuffer.Size == 1U);
  TEST_CHECK(RingBuffer.Head == 0U);
  TEST_CHECK(RingBuffer.Tail == 1U);
  TEST_CHECK(((uint32_t *)RingBuffer.Data)[0] == 55U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Push_TailWrap(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 55U;

  for (size_t Index = 0U; Index < 4U; Index++)
  {
    TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_OK);
  }

  TEST_CHECK(RingBuffer.Tail == 0U);
  TEST_CHECK(RingBuffer.Size == 4U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Pop_NullPtr(TestContextType * Context)
{
  TEST_CHECK(RingBuffer_Pop(NULL) == NCL_E_NULL_PTR);
}

static void Test_RingBuffer_Pop_InvalidData(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };

  TEST_CHECK(RingBuffer_Pop(&RingBuffer) == NCL_E_INVALID_ARGUMENT);
}

static void Test_RingBuffer_Pop_ZeroElementSize(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  uint32_t Value = 5U;

  RingBuffer.Data = (uint8_t *)&Value;
  TEST_CHECK(RingBuffer_Pop(&RingBuffer) == NCL_E_INVALID_ARGUMENT);
}

static void Test_RingBuffer_Pop_Empty(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(RingBuffer_Pop(&RingBuffer) == NCL_E_EMPTY);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Pop_Valid(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 55U;

  TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_OK);
  TEST_CHECK(RingBuffer_Pop(&RingBuffer) == NCL_E_OK);
  TEST_CHECK(RingBuffer.Head == 1U);
  TEST_CHECK(RingBuffer.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Pop_HeadWrap(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 55U;

  for (size_t Index = 0U; Index < 4U; Index++)
  {
    TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Value) == NCL_E_OK);
  }

  for (size_t Index = 0U; Index < 4U; Index++)
  {
    TEST_REQUIRE(RingBuffer_Pop(&RingBuffer) == NCL_E_OK);
  }

  TEST_CHECK(RingBuffer.Head == 0U);
  TEST_CHECK(RingBuffer.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_PushPop_FIFO_Wrap(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  uint32_t Values[] = { 10U, 20U, 30U, 40U, 50U, 60U };

  for (size_t Index = 0U; Index < 4U; Index++)
  {
    TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Values[Index]) == NCL_E_OK);
  }

  TEST_REQUIRE(RingBuffer_Pop(&RingBuffer) == NCL_E_OK);
  TEST_REQUIRE(RingBuffer_Pop(&RingBuffer) == NCL_E_OK);
  TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Values[4]) == NCL_E_OK);
  TEST_REQUIRE(RingBuffer_Push(&RingBuffer, &Values[5]) == NCL_E_OK);

  for (size_t Index = 2U; Index < 6U; Index++)
  {
    const uint32_t * Front = RingBuffer_FrontConst(&RingBuffer);

    TEST_REQUIRE(Front != NULL);
    TEST_CHECK(*Front == Values[Index]);
    TEST_REQUIRE(RingBuffer_Pop(&RingBuffer) == NCL_E_OK);
  }

  TEST_CHECK(RingBuffer_IsEmpty(&RingBuffer));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

/**
 * Debug
 */

static void Test_RingBuffer_Print_NullBuffer(TestContextType * Context)
{
  TEST_CHECK(RingBuffer_Print(NULL, PrintData) == NCL_E_NULL_PTR);
}

static void Test_RingBuffer_Print_NullCallback(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(RingBuffer_Print(&RingBuffer, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Print_InvalidData(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };

  TEST_CHECK(RingBuffer_Print(&RingBuffer, PrintData) == NCL_E_INVALID_ARGUMENT);
}

static void Test_RingBuffer_Print_ZeroElementSize(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  uint32_t Value = 5U;

  RingBuffer.Data = (uint8_t *)&Value;
  TEST_CHECK(RingBuffer_Print(&RingBuffer, PrintData) == NCL_E_INVALID_ARGUMENT);
}

static void Test_RingBuffer_Print_Valid(TestContextType * Context)
{
  RingBufferType RingBuffer = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(RingBuffer_Init(&RingBuffer, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  PrintCount = 0U;
  TEST_CHECK(RingBuffer_Print(&RingBuffer, PrintData) == NCL_E_OK);
  TEST_CHECK(PrintCount == RingBuffer.Capacity);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(RingBuffer_Deinit(&RingBuffer) == NCL_E_OK);
  }
}

static void Test_RingBuffer_Lifecycle(TestContextType * Context)
{
  TEST_RUN(Test_RingBuffer_Init_NullPtr);
  TEST_RUN(Test_RingBuffer_Init_ZeroElementSize);
  TEST_RUN(Test_RingBuffer_Init_ZeroCapacity);
  TEST_RUN(Test_RingBuffer_Init_Overflow);
  TEST_RUN(Test_RingBuffer_Init_Valid);
  TEST_RUN(Test_RingBuffer_Deinit_NullPtr);
  TEST_RUN(Test_RingBuffer_Deinit_Allocated);
  TEST_RUN(Test_RingBuffer_Deinit_NullData);
}

static void Test_RingBuffer_StateOperations(TestContextType * Context)
{
  TEST_RUN(Test_RingBuffer_Clear_NullPtr);
  TEST_RUN(Test_RingBuffer_Clear_Empty);
  TEST_RUN(Test_RingBuffer_Clear_Wrapped);
  TEST_RUN(Test_RingBuffer_Size_NullPtr);
  TEST_RUN(Test_RingBuffer_Size_Empty);
  TEST_RUN(Test_RingBuffer_Size_NonEmpty);
  TEST_RUN(Test_RingBuffer_IsEmpty_NullPtr);
  TEST_RUN(Test_RingBuffer_IsEmpty_Empty);
  TEST_RUN(Test_RingBuffer_IsEmpty_NonEmpty);
  TEST_RUN(Test_RingBuffer_IsFull_NullPtr);
  TEST_RUN(Test_RingBuffer_IsFull_NotFull);
  TEST_RUN(Test_RingBuffer_IsFull_Full);
}

static void Test_RingBuffer_EdgeOperations(TestContextType * Context)
{
  TEST_RUN(Test_RingBuffer_FrontConst_NullPtr);
  TEST_RUN(Test_RingBuffer_FrontConst_NullData);
  TEST_RUN(Test_RingBuffer_FrontConst_ZeroElementSize);
  TEST_RUN(Test_RingBuffer_FrontConst_Empty);
  TEST_RUN(Test_RingBuffer_FrontConst_Valid);
  TEST_RUN(Test_RingBuffer_FrontConst_Wrapped);
  TEST_RUN(Test_RingBuffer_Front_NullPtr);
  TEST_RUN(Test_RingBuffer_Front_NullData);
  TEST_RUN(Test_RingBuffer_Front_ZeroElementSize);
  TEST_RUN(Test_RingBuffer_Front_Empty);
  TEST_RUN(Test_RingBuffer_Front_Valid);
  TEST_RUN(Test_RingBuffer_Front_Wrapped);
  TEST_RUN(Test_RingBuffer_Push_NullBuffer);
  TEST_RUN(Test_RingBuffer_Push_NullValue);
  TEST_RUN(Test_RingBuffer_Push_InvalidData);
  TEST_RUN(Test_RingBuffer_Push_ZeroElementSize);
  TEST_RUN(Test_RingBuffer_Push_Full);
  TEST_RUN(Test_RingBuffer_Push_Valid);
  TEST_RUN(Test_RingBuffer_Push_TailWrap);
  TEST_RUN(Test_RingBuffer_Pop_NullPtr);
  TEST_RUN(Test_RingBuffer_Pop_InvalidData);
  TEST_RUN(Test_RingBuffer_Pop_ZeroElementSize);
  TEST_RUN(Test_RingBuffer_Pop_Empty);
  TEST_RUN(Test_RingBuffer_Pop_Valid);
  TEST_RUN(Test_RingBuffer_Pop_HeadWrap);
  TEST_RUN(Test_RingBuffer_PushPop_FIFO_Wrap);
}

static void Test_RingBuffer_Debug(TestContextType * Context)
{
  TEST_RUN(Test_RingBuffer_Print_NullBuffer);
  TEST_RUN(Test_RingBuffer_Print_NullCallback);
  TEST_RUN(Test_RingBuffer_Print_InvalidData);
  TEST_RUN(Test_RingBuffer_Print_ZeroElementSize);
  TEST_RUN(Test_RingBuffer_Print_Valid);
}

int main(void)
{
  TestContextType Context;

  TestUtils_Init(&Context);

  TestUtils_Run(&Context, Test_RingBuffer_Lifecycle, "RingBuffer Lifecycle");
  TestUtils_Run(&Context, Test_RingBuffer_StateOperations, "RingBuffer State Operations");
  TestUtils_Run(&Context, Test_RingBuffer_EdgeOperations, "RingBuffer Edge Operations");
  TestUtils_Run(&Context, Test_RingBuffer_Debug, "RingBuffer Debug");

  return TestUtils_Report(&Context);
}
