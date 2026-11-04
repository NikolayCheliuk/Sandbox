/**
 * VectorTest.c
 *
 * Regression tests for the public Vector API.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "Vector.h"
#include "TestUtils.h"

static NCLResultType Test_PrintData(const void * Data)
{
  (void)Data;
  return NCL_E_OK;
}

/**
 * Lifecycle
 */

static void Test_Vector_Init_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Vector_Init(NULL, sizeof(uint32_t)) == NCL_E_NULL_PTR);
}

static void Test_Vector_Init_InvalidArgument(TestContextType * Context)
{
  VectorType Vector = { 0 };

  TEST_CHECK(Vector_Init(&Vector, 0U) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Vector_Init_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_CHECK(Vector.Data == NULL);
  TEST_CHECK(Vector.ElementSize == sizeof(uint32_t));
  TEST_CHECK(Vector.Capacity == 0U);
  TEST_CHECK(Vector.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Deinit_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Vector_Deinit(NULL) == NCL_E_NULL_PTR);
}

static void Test_Vector_Deinit_Empty(TestContextType * Context)
{
  VectorType Vector;
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_REQUIRE(Vector_Deinit(&Vector) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(Vector.Data == NULL);
  TEST_CHECK(Vector.ElementSize == 0U);
  TEST_CHECK(Vector.Capacity == 0U);
  TEST_CHECK(Vector.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Deinit_Allocated(TestContextType * Context)
{
  VectorType Vector;
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_REQUIRE(Vector_Reserve(&Vector, 4U) == NCL_E_OK);
  TEST_CHECK(Vector.Data != NULL);
  TEST_REQUIRE(Vector_Deinit(&Vector) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(Vector.Data == NULL);
  TEST_CHECK(Vector.ElementSize == 0U);
  TEST_CHECK(Vector.Capacity == 0U);
  TEST_CHECK(Vector.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Lifecycle(TestContextType * Context)
{
  TEST_RUN(Test_Vector_Init_NullPtr);
  TEST_RUN(Test_Vector_Init_InvalidArgument);
  TEST_RUN(Test_Vector_Init_Valid);
  TEST_RUN(Test_Vector_Deinit_NullPtr);
  TEST_RUN(Test_Vector_Deinit_Empty);
  TEST_RUN(Test_Vector_Deinit_Allocated);
}

/**
 * State Operations
 */

static void Test_Vector_Reserve_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Vector_Reserve(NULL, 4U) == NCL_E_NULL_PTR);
}

static void Test_Vector_Reserve_InvalidArgument(TestContextType * Context)
{
  VectorType Vector = { 0 };

  TEST_CHECK(Vector_Reserve(&Vector, 4U) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Vector_Reserve_NoGrowth(TestContextType * Context)
{
  VectorType Vector;
  bool Initialized = false;
  uint8_t * Data = NULL;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_REQUIRE(Vector_Reserve(&Vector, 4U) == NCL_E_OK);
  Data = Vector.Data;
  TEST_CHECK(Vector_Reserve(&Vector, 2U) == NCL_E_OK);
  TEST_CHECK(Vector.Data == Data);
  TEST_CHECK(Vector.Capacity == 4U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Reserve_Overflow(TestContextType * Context)
{
  VectorType Vector;
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_CHECK(Vector_Reserve(&Vector, SIZE_MAX) == NCL_E_ALLOC_FAILED);
  TEST_CHECK(Vector.Data == NULL);
  TEST_CHECK(Vector.Capacity == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Reserve_Grow(TestContextType * Context)
{
  VectorType Vector;
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_REQUIRE(Vector_Reserve(&Vector, 4U) == NCL_E_OK);
  TEST_CHECK(Vector.Data != NULL);
  TEST_CHECK(Vector.Capacity == 4U);
  TEST_CHECK(Vector.Size == 0U);
  TEST_CHECK(((uint32_t *)Vector.Data)[0] == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Resize_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Vector_Resize(NULL, 4U) == NCL_E_NULL_PTR);
}

static void Test_Vector_Resize_InvalidArgument(TestContextType * Context)
{
  VectorType Vector = { 0 };

  TEST_CHECK(Vector_Resize(&Vector, 4U) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Vector_Resize_ReserveFailure(TestContextType * Context)
{
  VectorType Vector;
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_CHECK(Vector_Resize(&Vector, SIZE_MAX) == NCL_E_ALLOC_FAILED);
  TEST_CHECK(Vector.Size == 0U);
  TEST_CHECK(Vector.Capacity == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Resize_Grow(TestContextType * Context)
{
  VectorType Vector;
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_REQUIRE(Vector_PushBack(&Vector, &Value) == NCL_E_OK);
  TEST_REQUIRE(Vector_Resize(&Vector, 5U) == NCL_E_OK);
  TEST_CHECK(Vector.Size == 5U);
  TEST_CHECK(((uint32_t *)Vector.Data)[0] == 42U);

  for (size_t Index = 1U; Index < 5U; Index++)
  {
    TEST_CHECK(((uint32_t *)Vector.Data)[Index] == 0U);
  }

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Resize_Shrink(TestContextType * Context)
{
  VectorType Vector;
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_REQUIRE(Vector_Resize(&Vector, 4U) == NCL_E_OK);
  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);
  TEST_CHECK(Vector.Size == 2U);
  TEST_CHECK(Vector.Capacity >= 4U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Resize_GrowWithinCapacity(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Reserve(&Vector, 4U) == NCL_E_OK);

  TEST_CHECK(Vector_Resize(&Vector, 3U) == NCL_E_OK);
  TEST_CHECK(Vector.Size == 3U);
  TEST_CHECK(Vector.Capacity == 4U);
  TEST_CHECK(((uint32_t *)Vector.Data)[2] == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Clear_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Vector_Clear(NULL) == NCL_E_NULL_PTR);
}

static void Test_Vector_Clear_InvalidArgument(TestContextType * Context)
{
  VectorType Vector = { 0 };

  TEST_CHECK(Vector_Clear(&Vector) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Vector_Clear_Valid(TestContextType * Context)
{
  VectorType Vector;
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_REQUIRE(Vector_Resize(&Vector, 4U) == NCL_E_OK);
  TEST_REQUIRE(Vector_Clear(&Vector) == NCL_E_OK);
  TEST_CHECK(Vector.Size == 0U);
  TEST_CHECK(Vector.Capacity >= 4U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Clear_Empty(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_Clear(&Vector) == NCL_E_OK);
  TEST_CHECK(Vector.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Clear_InvalidInternalState(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  Vector.ElementSize = 0U;

  TEST_CHECK(Vector_Clear(&Vector) == NCL_E_INVALID_ARGUMENT);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Size_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Vector_Size(NULL) == 0U);
}

static void Test_Vector_Size_Valid(TestContextType * Context)
{
  VectorType Vector;
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_REQUIRE(Vector_Resize(&Vector, 3U) == NCL_E_OK);
  TEST_CHECK(Vector_Size(&Vector) == 3U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_IsEmpty_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Vector_IsEmpty(NULL));
}

static void Test_Vector_IsEmpty_Empty(TestContextType * Context)
{
  VectorType Vector;
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_CHECK(Vector_IsEmpty(&Vector));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_IsEmpty_NonEmpty(TestContextType * Context)
{
  VectorType Vector;
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;
  TEST_REQUIRE(Vector_Resize(&Vector, 1U) == NCL_E_OK);
  TEST_CHECK(!Vector_IsEmpty(&Vector));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_State(TestContextType * Context)
{
  TEST_RUN(Test_Vector_Reserve_NullPtr);
  TEST_RUN(Test_Vector_Reserve_InvalidArgument);
  TEST_RUN(Test_Vector_Reserve_NoGrowth);
  TEST_RUN(Test_Vector_Reserve_Overflow);
  TEST_RUN(Test_Vector_Reserve_Grow);
  TEST_RUN(Test_Vector_Resize_NullPtr);
  TEST_RUN(Test_Vector_Resize_InvalidArgument);
  TEST_RUN(Test_Vector_Resize_ReserveFailure);
  TEST_RUN(Test_Vector_Resize_Grow);
  TEST_RUN(Test_Vector_Resize_Shrink);
  TEST_RUN(Test_Vector_Resize_GrowWithinCapacity);
  TEST_RUN(Test_Vector_Clear_NullPtr);
  TEST_RUN(Test_Vector_Clear_InvalidArgument);
  TEST_RUN(Test_Vector_Clear_Valid);
  TEST_RUN(Test_Vector_Clear_Empty);
  TEST_RUN(Test_Vector_Clear_InvalidInternalState);
  TEST_RUN(Test_Vector_Size_NullPtr);
  TEST_RUN(Test_Vector_Size_Valid);
  TEST_RUN(Test_Vector_IsEmpty_NullPtr);
  TEST_RUN(Test_Vector_IsEmpty_Empty);
  TEST_RUN(Test_Vector_IsEmpty_NonEmpty);
}

/**
 * Iterators
 */

static void Test_VectorIterator_InitConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(VectorIterator_InitConst(NULL) == NCL_E_NULL_PTR);
}

static void Test_VectorIterator_InitConst_Valid(TestContextType * Context)
{
  ConstVectorIteratorType It = { 0 };
    It.Index = 7U;
    TEST_CHECK(VectorIterator_InitConst(&It) == NCL_E_OK);
    TEST_CHECK(It.Vector == NULL);
    TEST_CHECK(It.Index == 0U);
}

static void Test_VectorIterator_Init_NullPtr(TestContextType * Context)
{
  TEST_CHECK(VectorIterator_Init(NULL) == NCL_E_NULL_PTR);
}

static void Test_VectorIterator_Init_Valid(TestContextType * Context)
{
  VectorIteratorType It = { 0 };
    It.Index = 7U;
    TEST_CHECK(VectorIterator_Init(&It) == NCL_E_OK);
    TEST_CHECK(It.Vector == NULL);
    TEST_CHECK(It.Index == 0U);
}

static void Test_Vector_Iterator_NextConst_NullVector(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = { NULL, 0U };
  ConstVectorIteratorType Result = VectorIterator_NextConst(It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Result.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_NextConst_OutOfRange(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = { &Vector, 3U };
  ConstVectorIteratorType Result = VectorIterator_NextConst(It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Result.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_NextConst_Boundary(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = { &Vector, 2U };
  ConstVectorIteratorType Result = VectorIterator_NextConst(It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Result.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_NextConst_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = { &Vector, 0U };
  ConstVectorIteratorType Result = VectorIterator_NextConst(It);
  TEST_CHECK(Result.Vector == &Vector);
  TEST_CHECK(Result.Index == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Next_NullVector(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { NULL, 0U };
  VectorIteratorType Result = VectorIterator_Next(It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Result.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Next_OutOfRange(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 3U };
  VectorIteratorType Result = VectorIterator_Next(It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Result.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Next_Boundary(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 2U };
  VectorIteratorType Result = VectorIterator_Next(It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Result.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Next_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 0U };
  VectorIteratorType Result = VectorIterator_Next(It);
  TEST_CHECK(Result.Vector == &Vector);
  TEST_CHECK(Result.Index == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_PrevConst_NullVector(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = { NULL, 0U };
  ConstVectorIteratorType Result = VectorIterator_PrevConst(It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Result.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_PrevConst_OutOfRange(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = { &Vector, 3U };
  ConstVectorIteratorType Result = VectorIterator_PrevConst(It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Result.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_PrevConst_Boundary(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = { &Vector, 0U };
  ConstVectorIteratorType Result = VectorIterator_PrevConst(It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Result.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_PrevConst_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = { &Vector, 2U };
  ConstVectorIteratorType Result = VectorIterator_PrevConst(It);
  TEST_CHECK(Result.Vector == &Vector);
  TEST_CHECK(Result.Index == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Prev_NullVector(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { NULL, 0U };
  VectorIteratorType Result = VectorIterator_Prev(It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Result.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Prev_OutOfRange(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 3U };
  VectorIteratorType Result = VectorIterator_Prev(It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Result.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Prev_Boundary(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 0U };
  VectorIteratorType Result = VectorIterator_Prev(It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Result.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Prev_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 2U };
  VectorIteratorType Result = VectorIterator_Prev(It);
  TEST_CHECK(Result.Vector == &Vector);
  TEST_CHECK(Result.Index == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_DataConst_NullVector(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = { &Vector, 0U };
  It.Vector = NULL;
  TEST_CHECK(VectorIterator_DataConst(It) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_DataConst_NoStorage(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ConstVectorIteratorType It = { &Vector, 0U };

  TEST_CHECK(VectorIterator_DataConst(It) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_DataConst_InvalidElementSize(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = { &Vector, 0U };
  Vector.ElementSize = 0U;
  TEST_CHECK(VectorIterator_DataConst(It) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_DataConst_OutOfRange(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = { &Vector, 2U };

  TEST_CHECK(VectorIterator_DataConst(It) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_DataConst_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = { &Vector, 1U };

  TEST_CHECK(VectorIterator_DataConst(It) == Vector.Data + sizeof(uint32_t));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Data_NullVector(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 0U };
  It.Vector = NULL;
  TEST_CHECK(VectorIterator_Data(It) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Data_NoStorage(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  VectorIteratorType It = { &Vector, 0U };

  TEST_CHECK(VectorIterator_Data(It) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Data_InvalidElementSize(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 0U };
  Vector.ElementSize = 0U;
  TEST_CHECK(VectorIterator_Data(It) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Data_OutOfRange(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 2U };

  TEST_CHECK(VectorIterator_Data(It) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Data_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 1U };

  TEST_CHECK(VectorIterator_Data(It) == Vector.Data + sizeof(uint32_t));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_EqualConst_NullFirst(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It1 = { NULL, 0U };
  ConstVectorIteratorType It2 = { &Vector, 0U };
  TEST_CHECK(VectorIterator_EqualConst(It1, It2) == false);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_EqualConst_NullSecond(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It1 = { &Vector, 0U };
  ConstVectorIteratorType It2 = { NULL, 0U };
  TEST_CHECK(VectorIterator_EqualConst(It1, It2) == false);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_EqualConst_InvalidFirst(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It1 = { &Vector, 3U };
  ConstVectorIteratorType It2 = { &Vector, 0U };
  TEST_CHECK(VectorIterator_EqualConst(It1, It2) == false);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_EqualConst_InvalidSecond(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It1 = { &Vector, 0U };
  ConstVectorIteratorType It2 = { &Vector, 3U };
  TEST_CHECK(VectorIterator_EqualConst(It1, It2) == false);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_EqualConst_DifferentPosition(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It1 = { &Vector, 0U };
  ConstVectorIteratorType It2 = { &Vector, 1U };
  TEST_CHECK(VectorIterator_EqualConst(It1, It2) == false);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_EqualConst_Equal(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It1 = { &Vector, 0U };
  ConstVectorIteratorType It2 = { &Vector, 0U };
  TEST_CHECK(VectorIterator_EqualConst(It1, It2) == true);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_EqualConst_DifferentVector(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorType Other = { 0 };
  TEST_REQUIRE(Vector_Init(&Other, sizeof(uint32_t)) == NCL_E_OK);
  ConstVectorIteratorType It1 = { &Vector, 0U };
  ConstVectorIteratorType It2 = { &Other, 0U };
  TEST_CHECK(VectorIterator_EqualConst(It1, It2) == false);
  TEST_CHECK(Vector_Deinit(&Other) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Equal_NullFirst(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It1 = { NULL, 0U };
  VectorIteratorType It2 = { &Vector, 0U };
  TEST_CHECK(VectorIterator_Equal(It1, It2) == false);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Equal_NullSecond(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It1 = { &Vector, 0U };
  VectorIteratorType It2 = { NULL, 0U };
  TEST_CHECK(VectorIterator_Equal(It1, It2) == false);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Equal_InvalidFirst(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It1 = { &Vector, 3U };
  VectorIteratorType It2 = { &Vector, 0U };
  TEST_CHECK(VectorIterator_Equal(It1, It2) == false);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Equal_InvalidSecond(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It1 = { &Vector, 0U };
  VectorIteratorType It2 = { &Vector, 3U };
  TEST_CHECK(VectorIterator_Equal(It1, It2) == false);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Equal_DifferentPosition(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It1 = { &Vector, 0U };
  VectorIteratorType It2 = { &Vector, 1U };
  TEST_CHECK(VectorIterator_Equal(It1, It2) == false);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Equal_Equal(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It1 = { &Vector, 0U };
  VectorIteratorType It2 = { &Vector, 0U };
  TEST_CHECK(VectorIterator_Equal(It1, It2) == true);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterator_Equal_DifferentVector(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorType Other = { 0 };
  TEST_REQUIRE(Vector_Init(&Other, sizeof(uint32_t)) == NCL_E_OK);
  VectorIteratorType It1 = { &Vector, 0U };
  VectorIteratorType It2 = { &Other, 0U };
  TEST_CHECK(VectorIterator_Equal(It1, It2) == false);
  TEST_CHECK(Vector_Deinit(&Other) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_BeginConst_NullPtr(TestContextType * Context)
{

  ConstVectorIteratorType It = Vector_BeginConst(NULL);
  TEST_CHECK(It.Vector == NULL);
  TEST_CHECK(It.Index == 0U);
}

static void Test_Vector_BeginConst_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = Vector_BeginConst(&Vector);
  TEST_CHECK(It.Vector == &Vector);
  TEST_CHECK(It.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Begin_NullPtr(TestContextType * Context)
{

  VectorIteratorType It = Vector_Begin(NULL);
  TEST_CHECK(It.Vector == NULL);
  TEST_CHECK(It.Index == 0U);
}

static void Test_Vector_Begin_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = Vector_Begin(&Vector);
  TEST_CHECK(It.Vector == &Vector);
  TEST_CHECK(It.Index == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_EndConst_NullPtr(TestContextType * Context)
{

  ConstVectorIteratorType It = Vector_EndConst(NULL);
  TEST_CHECK(It.Vector == NULL);
  TEST_CHECK(It.Index == 0U);
}

static void Test_Vector_EndConst_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  ConstVectorIteratorType It = Vector_EndConst(&Vector);
  TEST_CHECK(It.Vector == &Vector);
  TEST_CHECK(It.Index == 2U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_End_NullPtr(TestContextType * Context)
{

  VectorIteratorType It = Vector_End(NULL);
  TEST_CHECK(It.Vector == NULL);
  TEST_CHECK(It.Index == 0U);
}

static void Test_Vector_End_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = Vector_End(&Vector);
  TEST_CHECK(It.Vector == &Vector);
  TEST_CHECK(It.Index == 2U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Iterators(TestContextType * Context)
{
  TEST_RUN(Test_VectorIterator_InitConst_NullPtr);
  TEST_RUN(Test_VectorIterator_InitConst_Valid);
  TEST_RUN(Test_VectorIterator_Init_NullPtr);
  TEST_RUN(Test_VectorIterator_Init_Valid);
  TEST_RUN(Test_Vector_Iterator_NextConst_NullVector);
  TEST_RUN(Test_Vector_Iterator_NextConst_OutOfRange);
  TEST_RUN(Test_Vector_Iterator_NextConst_Boundary);
  TEST_RUN(Test_Vector_Iterator_NextConst_Valid);
  TEST_RUN(Test_Vector_Iterator_Next_NullVector);
  TEST_RUN(Test_Vector_Iterator_Next_OutOfRange);
  TEST_RUN(Test_Vector_Iterator_Next_Boundary);
  TEST_RUN(Test_Vector_Iterator_Next_Valid);
  TEST_RUN(Test_Vector_Iterator_PrevConst_NullVector);
  TEST_RUN(Test_Vector_Iterator_PrevConst_OutOfRange);
  TEST_RUN(Test_Vector_Iterator_PrevConst_Boundary);
  TEST_RUN(Test_Vector_Iterator_PrevConst_Valid);
  TEST_RUN(Test_Vector_Iterator_Prev_NullVector);
  TEST_RUN(Test_Vector_Iterator_Prev_OutOfRange);
  TEST_RUN(Test_Vector_Iterator_Prev_Boundary);
  TEST_RUN(Test_Vector_Iterator_Prev_Valid);
  TEST_RUN(Test_Vector_Iterator_DataConst_NullVector);
  TEST_RUN(Test_Vector_Iterator_DataConst_NoStorage);
  TEST_RUN(Test_Vector_Iterator_DataConst_InvalidElementSize);
  TEST_RUN(Test_Vector_Iterator_DataConst_OutOfRange);
  TEST_RUN(Test_Vector_Iterator_DataConst_Valid);
  TEST_RUN(Test_Vector_Iterator_Data_NullVector);
  TEST_RUN(Test_Vector_Iterator_Data_NoStorage);
  TEST_RUN(Test_Vector_Iterator_Data_InvalidElementSize);
  TEST_RUN(Test_Vector_Iterator_Data_OutOfRange);
  TEST_RUN(Test_Vector_Iterator_Data_Valid);
  TEST_RUN(Test_Vector_Iterator_EqualConst_NullFirst);
  TEST_RUN(Test_Vector_Iterator_EqualConst_NullSecond);
  TEST_RUN(Test_Vector_Iterator_EqualConst_InvalidFirst);
  TEST_RUN(Test_Vector_Iterator_EqualConst_InvalidSecond);
  TEST_RUN(Test_Vector_Iterator_EqualConst_DifferentPosition);
  TEST_RUN(Test_Vector_Iterator_EqualConst_Equal);
  TEST_RUN(Test_Vector_Iterator_EqualConst_DifferentVector);
  TEST_RUN(Test_Vector_Iterator_Equal_NullFirst);
  TEST_RUN(Test_Vector_Iterator_Equal_NullSecond);
  TEST_RUN(Test_Vector_Iterator_Equal_InvalidFirst);
  TEST_RUN(Test_Vector_Iterator_Equal_InvalidSecond);
  TEST_RUN(Test_Vector_Iterator_Equal_DifferentPosition);
  TEST_RUN(Test_Vector_Iterator_Equal_Equal);
  TEST_RUN(Test_Vector_Iterator_Equal_DifferentVector);
  TEST_RUN(Test_Vector_BeginConst_NullPtr);
  TEST_RUN(Test_Vector_BeginConst_Valid);
  TEST_RUN(Test_Vector_Begin_NullPtr);
  TEST_RUN(Test_Vector_Begin_Valid);
  TEST_RUN(Test_Vector_EndConst_NullPtr);
  TEST_RUN(Test_Vector_EndConst_Valid);
  TEST_RUN(Test_Vector_End_NullPtr);
  TEST_RUN(Test_Vector_End_Valid);
}

/**
 * Access
 */

static void Test_Vector_DataAtConst_NullPtr(TestContextType * Context)
{

  TEST_CHECK(Vector_DataAtConst(NULL, 0U) == NULL);
}

static void Test_Vector_DataAtConst_NoStorage(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_DataAtConst(&Vector, 0U) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_DataAtConst_InvalidElementSize(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  Vector.ElementSize = 0U;

  TEST_CHECK(Vector_DataAtConst(&Vector, 0U) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_DataAtConst_OutOfRange(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_DataAtConst(&Vector, 2U) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_DataAtConst_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_DataAtConst(&Vector, 1U) == Vector.Data + sizeof(uint32_t));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_DataAt_NullPtr(TestContextType * Context)
{

  TEST_CHECK(Vector_DataAt(NULL, 0U) == NULL);
}

static void Test_Vector_DataAt_NoStorage(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_DataAt(&Vector, 0U) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_DataAt_InvalidElementSize(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  Vector.ElementSize = 0U;

  TEST_CHECK(Vector_DataAt(&Vector, 0U) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_DataAt_OutOfRange(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_DataAt(&Vector, 2U) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_DataAt_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_DataAt(&Vector, 1U) == Vector.Data + sizeof(uint32_t));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Get_NullVector(TestContextType * Context)
{
  uint32_t Value = 42U;
  TEST_CHECK(Vector_Get(NULL, 0U, &Value) == NCL_E_NULL_PTR);
}

static void Test_Vector_Get_NullValue(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_Get(&Vector, 0U, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Get_NoStorage(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_Get(&Vector, 0U, &Value) == NCL_E_INVALID_ARGUMENT);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Get_InvalidElementSize(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  Vector.ElementSize = 0U;

  TEST_CHECK(Vector_Get(&Vector, 0U, &Value) == NCL_E_INVALID_ARGUMENT);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Get_OutOfRange(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_Get(&Vector, 2U, &Value) == NCL_E_OUT_OF_RANGE);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Get_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_Get(&Vector, 1U, &Value) == NCL_E_OK);
  TEST_CHECK(Value == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Set_NullVector(TestContextType * Context)
{
  uint32_t Value = 42U;
  TEST_CHECK(Vector_Set(NULL, 0U, &Value) == NCL_E_NULL_PTR);
}

static void Test_Vector_Set_NullValue(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_Set(&Vector, 0U, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Set_NoStorage(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_Set(&Vector, 0U, &Value) == NCL_E_INVALID_ARGUMENT);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Set_InvalidElementSize(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  Vector.ElementSize = 0U;

  TEST_CHECK(Vector_Set(&Vector, 0U, &Value) == NCL_E_INVALID_ARGUMENT);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Set_OutOfRange(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_Set(&Vector, 2U, &Value) == NCL_E_OUT_OF_RANGE);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Set_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_Set(&Vector, 1U, &Value) == NCL_E_OK);
  TEST_CHECK(((uint32_t *)Vector.Data)[1] == 42U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Access(TestContextType * Context)
{
  TEST_RUN(Test_Vector_DataAtConst_NullPtr);
  TEST_RUN(Test_Vector_DataAtConst_NoStorage);
  TEST_RUN(Test_Vector_DataAtConst_InvalidElementSize);
  TEST_RUN(Test_Vector_DataAtConst_OutOfRange);
  TEST_RUN(Test_Vector_DataAtConst_Valid);
  TEST_RUN(Test_Vector_DataAt_NullPtr);
  TEST_RUN(Test_Vector_DataAt_NoStorage);
  TEST_RUN(Test_Vector_DataAt_InvalidElementSize);
  TEST_RUN(Test_Vector_DataAt_OutOfRange);
  TEST_RUN(Test_Vector_DataAt_Valid);
  TEST_RUN(Test_Vector_Get_NullVector);
  TEST_RUN(Test_Vector_Get_NullValue);
  TEST_RUN(Test_Vector_Get_NoStorage);
  TEST_RUN(Test_Vector_Get_InvalidElementSize);
  TEST_RUN(Test_Vector_Get_OutOfRange);
  TEST_RUN(Test_Vector_Get_Valid);
  TEST_RUN(Test_Vector_Set_NullVector);
  TEST_RUN(Test_Vector_Set_NullValue);
  TEST_RUN(Test_Vector_Set_NoStorage);
  TEST_RUN(Test_Vector_Set_InvalidElementSize);
  TEST_RUN(Test_Vector_Set_OutOfRange);
  TEST_RUN(Test_Vector_Set_Valid);
}

/**
 * Modification
 */

static void Test_Vector_Insert_NullVector(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { NULL, 0U };
  VectorIteratorType Result = Vector_Insert(NULL, It, &Value);
  TEST_CHECK(Result.Vector == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Insert_NullValue(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 0U };
  VectorIteratorType Result = Vector_Insert(&Vector, It, NULL);
  TEST_CHECK(Result.Vector == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Insert_InvalidElementSize(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);
  Vector.ElementSize = 0U;

  VectorIteratorType It = { &Vector, 0U };
  VectorIteratorType Result = Vector_Insert(&Vector, It, &Value);
  TEST_CHECK(Result.Vector == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Insert_WrongVector(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorType Other = { 0 };
  TEST_REQUIRE(Vector_Init(&Other, sizeof(uint32_t)) == NCL_E_OK);
  VectorIteratorType It = { &Other, 0U };
  VectorIteratorType Result = Vector_Insert(&Vector, It, &Value);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Vector_Deinit(&Other) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Insert_OutOfRange(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 3U };
  VectorIteratorType Result = Vector_Insert(&Vector, It, &Value);
  TEST_CHECK(Result.Vector == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Insert_Append(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 2U };
  VectorIteratorType Result = Vector_Insert(&Vector, It, &Value);
  TEST_CHECK(Result.Vector == &Vector);
  TEST_CHECK(Result.Index == 2U);
  TEST_CHECK(Vector.Size == 3U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Insert_Middle(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 0U };
  VectorIteratorType Result = Vector_Insert(&Vector, It, &Value);
  TEST_CHECK(Result.Vector == &Vector);
  TEST_CHECK(Result.Index == 0U);
  TEST_CHECK(Vector.Size == 3U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Insert_NoGrowth(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);
  TEST_REQUIRE(Vector_Reserve(&Vector, 8U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 0U };
  VectorIteratorType Result = Vector_Insert(&Vector, It, &Value);
  TEST_CHECK(Result.Vector == &Vector);
  TEST_CHECK(Result.Index == 0U);
  TEST_CHECK(Vector.Size == 3U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Erase_NullVector(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { NULL, 0U };
  VectorIteratorType Result = Vector_Erase(NULL, It);
  TEST_CHECK(Result.Vector == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Erase_NoStorage(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  VectorIteratorType It = { &Vector, 0U };
  VectorIteratorType Result = Vector_Erase(&Vector, It);
  TEST_CHECK(Result.Vector == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Erase_InvalidElementSize(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);
  Vector.ElementSize = 0U;

  VectorIteratorType It = { &Vector, 0U };
  VectorIteratorType Result = Vector_Erase(&Vector, It);
  TEST_CHECK(Result.Vector == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Erase_WrongVector(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorType Other = { 0 };
  TEST_REQUIRE(Vector_Init(&Other, sizeof(uint32_t)) == NCL_E_OK);
  VectorIteratorType It = { &Other, 0U };
  VectorIteratorType Result = Vector_Erase(&Vector, It);
  TEST_CHECK(Result.Vector == NULL);
  TEST_CHECK(Vector_Deinit(&Other) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Erase_OutOfRange(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 3U };
  VectorIteratorType Result = Vector_Erase(&Vector, It);
  TEST_CHECK(Result.Vector == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Erase_Last(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 1U };
  VectorIteratorType Result = Vector_Erase(&Vector, It);
  TEST_CHECK(Result.Vector == &Vector);
  TEST_CHECK(Result.Index == 1U);
  TEST_CHECK(Vector.Size == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Erase_Middle(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  VectorIteratorType It = { &Vector, 0U };
  VectorIteratorType Result = Vector_Erase(&Vector, It);
  TEST_CHECK(Result.Vector == &Vector);
  TEST_CHECK(Result.Index == 0U);
  TEST_CHECK(Vector.Size == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Modification(TestContextType * Context)
{
  TEST_RUN(Test_Vector_Insert_NullVector);
  TEST_RUN(Test_Vector_Insert_NullValue);
  TEST_RUN(Test_Vector_Insert_InvalidElementSize);
  TEST_RUN(Test_Vector_Insert_WrongVector);
  TEST_RUN(Test_Vector_Insert_OutOfRange);
  TEST_RUN(Test_Vector_Insert_Append);
  TEST_RUN(Test_Vector_Insert_Middle);
  TEST_RUN(Test_Vector_Insert_NoGrowth);
  TEST_RUN(Test_Vector_Erase_NullVector);
  TEST_RUN(Test_Vector_Erase_NoStorage);
  TEST_RUN(Test_Vector_Erase_InvalidElementSize);
  TEST_RUN(Test_Vector_Erase_WrongVector);
  TEST_RUN(Test_Vector_Erase_OutOfRange);
  TEST_RUN(Test_Vector_Erase_Last);
  TEST_RUN(Test_Vector_Erase_Middle);
}

/**
 * Edge Operations
 */

static void Test_Vector_FrontConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Vector_FrontConst(NULL) == NULL);
}

static void Test_Vector_FrontConst_Empty(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_FrontConst(&Vector) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_FrontConst_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_FrontConst(&Vector) == Vector.Data);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Front_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Vector_Front(NULL) == NULL);
}

static void Test_Vector_Front_Empty(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_Front(&Vector) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Front_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_Front(&Vector) == Vector.Data);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_BackConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Vector_BackConst(NULL) == NULL);
}

static void Test_Vector_BackConst_Empty(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_BackConst(&Vector) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_BackConst_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_BackConst(&Vector) == Vector.Data + sizeof(uint32_t));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Back_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Vector_Back(NULL) == NULL);
}

static void Test_Vector_Back_Empty(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_Back(&Vector) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Back_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_Back(&Vector) == Vector.Data + sizeof(uint32_t));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_PushFront_NullVector(TestContextType * Context)
{
  uint32_t Value = 42U;
  TEST_CHECK(Vector_PushFront(NULL, &Value) == NCL_E_NULL_PTR);
}

static void Test_Vector_PushFront_NullValue(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_PushFront(&Vector, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_PushFront_InvalidState(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  Vector.ElementSize = 0U;

  TEST_CHECK(Vector_PushFront(&Vector, &Value) == NCL_E_NOT_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_PushFront_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_PushFront(&Vector, &Value) == NCL_E_OK);
  TEST_CHECK(Vector.Size == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_PushBack_NullVector(TestContextType * Context)
{
  uint32_t Value = 42U;
  TEST_CHECK(Vector_PushBack(NULL, &Value) == NCL_E_NULL_PTR);
}

static void Test_Vector_PushBack_NullValue(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_PushBack(&Vector, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_PushBack_InvalidState(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  Vector.ElementSize = 0U;

  TEST_CHECK(Vector_PushBack(&Vector, &Value) == NCL_E_NOT_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_PushBack_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_PushBack(&Vector, &Value) == NCL_E_OK);
  TEST_CHECK(Vector.Size == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_PopFront_NullPtr(TestContextType * Context)
{

  TEST_CHECK(Vector_PopFront(NULL) == NCL_E_NULL_PTR);
}

static void Test_Vector_PopFront_Empty(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_PopFront(&Vector) == NCL_E_EMPTY);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_PopFront_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_PopFront(&Vector) == NCL_E_OK);
  TEST_CHECK(Vector.Size == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_PopBack_NullPtr(TestContextType * Context)
{

  TEST_CHECK(Vector_PopBack(NULL) == NCL_E_NULL_PTR);
}

static void Test_Vector_PopBack_Empty(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_PopBack(&Vector) == NCL_E_EMPTY);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_PopBack_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Vector_Resize(&Vector, 2U) == NCL_E_OK);

  TEST_CHECK(Vector_PopBack(&Vector) == NCL_E_OK);
  TEST_CHECK(Vector.Size == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Edge(TestContextType * Context)
{
  TEST_RUN(Test_Vector_FrontConst_NullPtr);
  TEST_RUN(Test_Vector_FrontConst_Empty);
  TEST_RUN(Test_Vector_FrontConst_Valid);
  TEST_RUN(Test_Vector_Front_NullPtr);
  TEST_RUN(Test_Vector_Front_Empty);
  TEST_RUN(Test_Vector_Front_Valid);
  TEST_RUN(Test_Vector_BackConst_NullPtr);
  TEST_RUN(Test_Vector_BackConst_Empty);
  TEST_RUN(Test_Vector_BackConst_Valid);
  TEST_RUN(Test_Vector_Back_NullPtr);
  TEST_RUN(Test_Vector_Back_Empty);
  TEST_RUN(Test_Vector_Back_Valid);
  TEST_RUN(Test_Vector_PushFront_NullVector);
  TEST_RUN(Test_Vector_PushFront_NullValue);
  TEST_RUN(Test_Vector_PushFront_InvalidState);
  TEST_RUN(Test_Vector_PushFront_Valid);
  TEST_RUN(Test_Vector_PushBack_NullVector);
  TEST_RUN(Test_Vector_PushBack_NullValue);
  TEST_RUN(Test_Vector_PushBack_InvalidState);
  TEST_RUN(Test_Vector_PushBack_Valid);
  TEST_RUN(Test_Vector_PopFront_NullPtr);
  TEST_RUN(Test_Vector_PopFront_Empty);
  TEST_RUN(Test_Vector_PopFront_Valid);
  TEST_RUN(Test_Vector_PopBack_NullPtr);
  TEST_RUN(Test_Vector_PopBack_Empty);
  TEST_RUN(Test_Vector_PopBack_Valid);
}

/**
 * Debug
 */

static void Test_Vector_Print_NullVector(TestContextType * Context)
{

  TEST_CHECK(Vector_Print(NULL, Test_PrintData) == NCL_E_NULL_PTR);
}

static void Test_Vector_Print_NullCallback(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_Print(&Vector, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Print_InvalidState(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  Vector.ElementSize = 0U;

  TEST_CHECK(Vector_Print(&Vector, Test_PrintData) == NCL_E_INVALID_ARGUMENT);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Print_Valid(TestContextType * Context)
{
  VectorType Vector = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Vector_Init(&Vector, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Vector_Print(&Vector, Test_PrintData) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Vector_Deinit(&Vector) == NCL_E_OK);
  }
}

static void Test_Vector_Debug(TestContextType * Context)
{
  TEST_RUN(Test_Vector_Print_NullVector);
  TEST_RUN(Test_Vector_Print_NullCallback);
  TEST_RUN(Test_Vector_Print_InvalidState);
  TEST_RUN(Test_Vector_Print_Valid);
}

int main(void)
{
  TestContextType Context;

  TestUtils_Init(&Context);

  TestUtils_Run(&Context, Test_Vector_Lifecycle, "Vector Lifecycle");
  TestUtils_Run(&Context, Test_Vector_State, "Vector State");
  TestUtils_Run(&Context, Test_Vector_Iterators, "Vector Iterators");
  TestUtils_Run(&Context, Test_Vector_Access, "Vector Access");
  TestUtils_Run(&Context, Test_Vector_Modification, "Vector Modification");
  TestUtils_Run(&Context, Test_Vector_Edge, "Vector Edge");
  TestUtils_Run(&Context, Test_Vector_Debug, "Vector Debug");

  return TestUtils_Report(&Context);
}
