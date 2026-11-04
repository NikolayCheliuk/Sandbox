/**
 * HeapTest.c
 *
 * Regression tests for the public Heap API.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "Heap.h"
#include "TestUtils.h"

static NCLResultType Test_PrintUInt32(const void * Value)
{
  return Value == NULL ? NCL_E_NULL_PTR : NCL_E_OK;
}

/**
 * Lifecycle
 */

static void Test_Heap_Init_NullHeap(TestContextType * Context)
{
  TEST_CHECK(Heap_Init(NULL, sizeof(uint32_t), LessUInt32) == NCL_E_NULL_PTR);
}

static void Test_Heap_Init_NullOrder(TestContextType * Context)
{
  HeapType Heap = { 0 };

  TEST_CHECK(Heap_Init(&Heap, sizeof(uint32_t), NULL) == NCL_E_NULL_PTR);
}

static void Test_Heap_Init_InvalidElementSize(TestContextType * Context)
{
  HeapType Heap = { 0 };

  TEST_CHECK(Heap_Init(&Heap, 0U, LessUInt32) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Heap_Init_Valid(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Heap.Vector.Data == NULL);
  TEST_CHECK(Heap.Vector.ElementSize == sizeof(uint32_t));
  TEST_CHECK(Heap.Vector.Size == 0U);
  TEST_CHECK(Heap.Vector.Capacity == 0U);
  TEST_CHECK(Heap.OrderFunction == LessUInt32);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Deinit_NullHeap(TestContextType * Context)
{
  TEST_CHECK(Heap_Deinit(NULL) == NCL_E_NULL_PTR);
}

static void Test_Heap_Deinit_Empty(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Deinit(&Heap) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(Heap.Vector.Data == NULL);
  TEST_CHECK(Heap.Vector.Size == 0U);
  TEST_CHECK(Heap.Vector.ElementSize == 0U);
  TEST_CHECK(Heap.OrderFunction == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Deinit_Allocated(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value8 = 8U;
  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_REQUIRE(Heap_Deinit(&Heap) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(Heap.Vector.Data == NULL);
  TEST_CHECK(Heap.Vector.Size == 0U);
  TEST_CHECK(Heap.OrderFunction == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Lifecycle(TestContextType * Context)
{
  TEST_RUN(Test_Heap_Init_NullHeap);
  TEST_RUN(Test_Heap_Init_NullOrder);
  TEST_RUN(Test_Heap_Init_InvalidElementSize);
  TEST_RUN(Test_Heap_Init_Valid);
  TEST_RUN(Test_Heap_Deinit_NullHeap);
  TEST_RUN(Test_Heap_Deinit_Empty);
  TEST_RUN(Test_Heap_Deinit_Allocated);
}

/**
 * State Operations
 */

static void Test_Heap_Clear_NullHeap(TestContextType * Context)
{
  TEST_CHECK(Heap_Clear(NULL) == NCL_E_NULL_PTR);
}

static void Test_Heap_Clear_InvalidVector(TestContextType * Context)
{
  HeapType Heap = { 0 };

  TEST_CHECK(Heap_Clear(&Heap) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Heap_Clear_Empty(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Heap_Clear(&Heap) == NCL_E_OK);
  TEST_CHECK(Heap_Size(&Heap) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Clear_NonEmpty(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value4 = 4U;
  uint32_t Value8 = 8U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_CHECK(Heap_Clear(&Heap) == NCL_E_OK);
  TEST_CHECK(Heap_Size(&Heap) == 0U);
  TEST_CHECK(Heap.Vector.Capacity >= 2U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Size_NullHeap(TestContextType * Context)
{
  TEST_CHECK(Heap_Size(NULL) == 0U);
}

static void Test_Heap_Size_Empty(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Heap_Size(&Heap) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Size_NonEmpty(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value4 = 4U;
  uint32_t Value8 = 8U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_CHECK(Heap_Size(&Heap) == 2U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_IsEmpty_NullHeap(TestContextType * Context)
{
  TEST_CHECK(Heap_IsEmpty(NULL));
}

static void Test_Heap_IsEmpty_Empty(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Heap_IsEmpty(&Heap));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_IsEmpty_NonEmpty(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value4 = 4U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_CHECK(!Heap_IsEmpty(&Heap));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_State(TestContextType * Context)
{
  TEST_RUN(Test_Heap_Clear_NullHeap);
  TEST_RUN(Test_Heap_Clear_InvalidVector);
  TEST_RUN(Test_Heap_Clear_Empty);
  TEST_RUN(Test_Heap_Clear_NonEmpty);
  TEST_RUN(Test_Heap_Size_NullHeap);
  TEST_RUN(Test_Heap_Size_Empty);
  TEST_RUN(Test_Heap_Size_NonEmpty);
  TEST_RUN(Test_Heap_IsEmpty_NullHeap);
  TEST_RUN(Test_Heap_IsEmpty_Empty);
  TEST_RUN(Test_Heap_IsEmpty_NonEmpty);
}

/**
 * Edge Operations
 */

static void Test_Heap_Top_NullHeap(TestContextType * Context)
{
  TEST_CHECK(Heap_Top(NULL) == NULL);
}

static void Test_Heap_Top_Empty(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Heap_Top(&Heap) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Top_NonEmpty(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value4 = 4U;
  uint32_t Value8 = 8U;
  uint32_t Value2 = 2U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value2) == NCL_E_OK);
  const uint32_t * Top = Heap_Top(&Heap);
  TEST_CHECK(Top != NULL);
  if (Top != NULL)
  {
    TEST_CHECK(*Top == 8U);
  }

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Push_NullHeap(TestContextType * Context)
{
  uint32_t Value = 4U;

  TEST_CHECK(Heap_Push(NULL, &Value) == NCL_E_NULL_PTR);
}

static void Test_Heap_Push_NullValue(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Heap_Push(&Heap, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Push_InvalidVector(TestContextType * Context)
{
  HeapType Heap = { 0 };
  uint32_t Value = 4U;

  TEST_CHECK(Heap_Push(&Heap, &Value) == NCL_E_NOT_OK);
}

static void Test_Heap_Push_First(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value4 = 4U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_CHECK(Heap_Size(&Heap) == 1U);
  TEST_CHECK(*(const uint32_t *)Heap_Top(&Heap) == 4U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Push_SiftUpSwap(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value4 = 4U;
  uint32_t Value8 = 8U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)Heap_Top(&Heap) == 8U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Push_SiftUpNoSwap(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value8 = 8U;
  uint32_t Value4 = 4U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)Heap_Top(&Heap) == 8U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Push_SiftUpMultipleLevels(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value2 = 2U;
  uint32_t Value4 = 4U;
  uint32_t Value6 = 6U;
  uint32_t Value8 = 8U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value2) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value6) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)Heap_Top(&Heap) == 8U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Push_MinHeap(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value8 = 8U;
  uint32_t Value2 = 2U;
  uint32_t Value4 = 4U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), GreaterUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value2) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)Heap_Top(&Heap) == 2U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Pop_NullHeap(TestContextType * Context)
{
  TEST_CHECK(Heap_Pop(NULL) == NCL_E_NULL_PTR);
}

static void Test_Heap_Pop_Empty(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Heap_Pop(&Heap) == NCL_E_EMPTY);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Pop_Single(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value4 = 4U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_CHECK(Heap_Pop(&Heap) == NCL_E_OK);
  TEST_CHECK(Heap_IsEmpty(&Heap));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Pop_TwoElements(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value4 = 4U;
  uint32_t Value8 = 8U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_CHECK(Heap_Pop(&Heap) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)Heap_Top(&Heap) == 4U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Pop_SiftDownChooseLeft(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value10 = 10U;
  uint32_t Value9 = 9U;
  uint32_t Value8 = 8U;
  uint32_t Value7 = 7U;
  uint32_t Value6 = 6U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value10) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value9) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value7) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value6) == NCL_E_OK);
  TEST_CHECK(Heap_Pop(&Heap) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)Heap_Top(&Heap) == 9U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Pop_SiftDownChooseRight(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value10 = 10U;
  uint32_t Value8 = 8U;
  uint32_t Value9 = 9U;
  uint32_t Value6 = 6U;
  uint32_t Value7 = 7U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value10) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value9) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value6) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value7) == NCL_E_OK);
  TEST_CHECK(Heap_Pop(&Heap) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)Heap_Top(&Heap) == 9U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Pop_OrderingAllElements(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value4 = 4U;
  uint32_t Value8 = 8U;
  uint32_t Value2 = 2U;
  uint32_t Value10 = 10U;
  uint32_t Value6 = 6U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value2) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value10) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value6) == NCL_E_OK);
  uint32_t Expected[] = { 10U, 8U, 6U, 4U, 2U };

  for (size_t Index = 0U; Index < 5U; Index++)
  {
    TEST_REQUIRE(Heap_Top(&Heap) != NULL);
    TEST_CHECK(*(const uint32_t *)Heap_Top(&Heap) == Expected[Index]);
    TEST_REQUIRE(Heap_Pop(&Heap) == NCL_E_OK);
  }

  TEST_CHECK(Heap_IsEmpty(&Heap));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Edge(TestContextType * Context)
{
  TEST_RUN(Test_Heap_Top_NullHeap);
  TEST_RUN(Test_Heap_Top_Empty);
  TEST_RUN(Test_Heap_Top_NonEmpty);
  TEST_RUN(Test_Heap_Push_NullHeap);
  TEST_RUN(Test_Heap_Push_NullValue);
  TEST_RUN(Test_Heap_Push_InvalidVector);
  TEST_RUN(Test_Heap_Push_First);
  TEST_RUN(Test_Heap_Push_SiftUpSwap);
  TEST_RUN(Test_Heap_Push_SiftUpNoSwap);
  TEST_RUN(Test_Heap_Push_SiftUpMultipleLevels);
  TEST_RUN(Test_Heap_Push_MinHeap);
  TEST_RUN(Test_Heap_Pop_NullHeap);
  TEST_RUN(Test_Heap_Pop_Empty);
  TEST_RUN(Test_Heap_Pop_Single);
  TEST_RUN(Test_Heap_Pop_TwoElements);
  TEST_RUN(Test_Heap_Pop_SiftDownChooseLeft);
  TEST_RUN(Test_Heap_Pop_SiftDownChooseRight);
  TEST_RUN(Test_Heap_Pop_OrderingAllElements);
}

/**
 * Debug
 */

static void Test_Heap_Print_NullHeap(TestContextType * Context)
{
  TEST_CHECK(Heap_Print(NULL, Test_PrintUInt32) == NCL_E_NULL_PTR);
}

static void Test_Heap_Print_NullCallback(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Heap_Print(&Heap, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Print_InvalidVector(TestContextType * Context)
{
  HeapType Heap = { 0 };

  TEST_CHECK(Heap_Print(&Heap, Test_PrintUInt32) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Heap_Print_Empty(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Heap_Print(&Heap, Test_PrintUInt32) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Print_NonEmpty(TestContextType * Context)
{
  HeapType Heap = { 0 };
  bool Initialized = false;
  uint32_t Value4 = 4U;
  uint32_t Value8 = 8U;

  TEST_REQUIRE(Heap_Init(&Heap, sizeof(uint32_t), LessUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Heap_Push(&Heap, &Value4) == NCL_E_OK);
  TEST_REQUIRE(Heap_Push(&Heap, &Value8) == NCL_E_OK);
  TEST_CHECK(Heap_Print(&Heap, Test_PrintUInt32) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Heap_Deinit(&Heap) == NCL_E_OK);
  }
}

static void Test_Heap_Debug(TestContextType * Context)
{
  TEST_RUN(Test_Heap_Print_NullHeap);
  TEST_RUN(Test_Heap_Print_NullCallback);
  TEST_RUN(Test_Heap_Print_InvalidVector);
  TEST_RUN(Test_Heap_Print_Empty);
  TEST_RUN(Test_Heap_Print_NonEmpty);
}

int main(void)
{
  TestContextType Context;

  TestUtils_Init(&Context);

  TestUtils_Run(&Context, Test_Heap_Lifecycle, "Heap Lifecycle");
  TestUtils_Run(&Context, Test_Heap_State, "Heap State");
  TestUtils_Run(&Context, Test_Heap_Edge, "Heap Edge");
  TestUtils_Run(&Context, Test_Heap_Debug, "Heap Debug");

  return TestUtils_Report(&Context);
}
