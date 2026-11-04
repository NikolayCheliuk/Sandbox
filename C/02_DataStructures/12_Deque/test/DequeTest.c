/**
 * DequeTest.c
 *
 * Regression tests for the public Deque API.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "Deque.h"
#include "TestUtils.h"

/**
 * Lifecycle
 */

static void Test_Deque_Init_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Deque_Init(NULL, sizeof(uint32_t)) == NCL_E_NULL_PTR);
}

static void Test_Deque_Init_ZeroElementSize(TestContextType * Context)
{
  DequeType Deque = { 0 };

  TEST_CHECK(Deque_Init(&Deque, 0U) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Deque_Init_Overflow(TestContextType * Context)
{
  DequeType Deque = { 0 };

  TEST_CHECK(Deque_Init(&Deque, SIZE_MAX) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Deque_Init_Valid(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque.List.ElementSize == sizeof(uint32_t));
  TEST_CHECK(Deque.List.Size == 0U);
  TEST_CHECK(Deque.List.Sentinel.Next == &Deque.List.Sentinel);
  TEST_CHECK(Deque.List.Sentinel.Prev == &Deque.List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_Deinit_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Deque_Deinit(NULL) == NCL_E_NULL_PTR);
}

static void Test_Deque_Deinit_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_Deinit(&Deque) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(Deque.List.ElementSize == 0U);
  TEST_CHECK(Deque.List.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_Deinit_NonEmpty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;
  uint32_t B = 20U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushBack(&Deque, &B) == NCL_E_OK);
  TEST_REQUIRE(Deque_Deinit(&Deque) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(Deque.List.ElementSize == 0U);
  TEST_CHECK(Deque.List.Size == 0U);
  TEST_CHECK(Deque.List.Sentinel.Next == &Deque.List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}


static void Test_Deque_Lifecycle(TestContextType * Context)
{
  TEST_RUN(Test_Deque_Init_NullPtr);
  TEST_RUN(Test_Deque_Init_ZeroElementSize);
  TEST_RUN(Test_Deque_Init_Overflow);
  TEST_RUN(Test_Deque_Init_Valid);
  TEST_RUN(Test_Deque_Deinit_NullPtr);
  TEST_RUN(Test_Deque_Deinit_Empty);
  TEST_RUN(Test_Deque_Deinit_NonEmpty);
}

/**
 * State Operations
 */

static void Test_Deque_Clear_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Deque_Clear(NULL) == NCL_E_NULL_PTR);
}

static void Test_Deque_Clear_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_Clear(&Deque) == NCL_E_OK);
  TEST_CHECK(Deque_Size(&Deque) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_Clear_NonEmpty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;
  uint32_t B = 20U;
  uint32_t C = 30U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushBack(&Deque, &B) == NCL_E_OK);
  TEST_CHECK(Deque_Clear(&Deque) == NCL_E_OK);
  TEST_CHECK(Deque_Size(&Deque) == 0U);
  TEST_CHECK(Deque_Front(&Deque) == NULL);
  TEST_REQUIRE(Deque_PushBack(&Deque, &C) == NCL_E_OK);
  TEST_CHECK(*(uint32_t *)Deque_Front(&Deque) == C);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_Size_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Deque_Size(NULL) == 0U);
}

static void Test_Deque_Size_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_Size(&Deque) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_Size_NonEmpty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;
  uint32_t B = 20U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushBack(&Deque, &B) == NCL_E_OK);
  TEST_CHECK(Deque_Size(&Deque) == 2U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_IsEmpty_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Deque_IsEmpty(NULL));
}

static void Test_Deque_IsEmpty_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_IsEmpty(&Deque));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_IsEmpty_NonEmpty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_CHECK(!Deque_IsEmpty(&Deque));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}


static void Test_Deque_State(TestContextType * Context)
{
  TEST_RUN(Test_Deque_Clear_NullPtr);
  TEST_RUN(Test_Deque_Clear_Empty);
  TEST_RUN(Test_Deque_Clear_NonEmpty);
  TEST_RUN(Test_Deque_Size_NullPtr);
  TEST_RUN(Test_Deque_Size_Empty);
  TEST_RUN(Test_Deque_Size_NonEmpty);
  TEST_RUN(Test_Deque_IsEmpty_NullPtr);
  TEST_RUN(Test_Deque_IsEmpty_Empty);
  TEST_RUN(Test_Deque_IsEmpty_NonEmpty);
}

/**
 * Edge Operations
 */

static void Test_Deque_FrontConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Deque_FrontConst(NULL) == NULL);
}

static void Test_Deque_FrontConst_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_FrontConst(&Deque) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_FrontConst_NonEmpty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;
  uint32_t B = 20U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushBack(&Deque, &B) == NCL_E_OK);
  TEST_CHECK(Deque_FrontConst(&Deque) != NULL);
  TEST_CHECK(*(const uint32_t *)Deque_FrontConst(&Deque) == A);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_Front_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Deque_Front(NULL) == NULL);
}

static void Test_Deque_Front_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_Front(&Deque) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_Front_NonEmpty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;
  uint32_t B = 20U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushBack(&Deque, &B) == NCL_E_OK);
  TEST_CHECK(Deque_Front(&Deque) != NULL);
  TEST_CHECK(*(const uint32_t *)Deque_Front(&Deque) == A);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_BackConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Deque_BackConst(NULL) == NULL);
}

static void Test_Deque_BackConst_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_BackConst(&Deque) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_BackConst_NonEmpty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;
  uint32_t B = 20U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushBack(&Deque, &B) == NCL_E_OK);
  TEST_CHECK(Deque_BackConst(&Deque) != NULL);
  TEST_CHECK(*(const uint32_t *)Deque_BackConst(&Deque) == B);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_Back_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Deque_Back(NULL) == NULL);
}

static void Test_Deque_Back_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_Back(&Deque) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_Back_NonEmpty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;
  uint32_t B = 20U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushBack(&Deque, &B) == NCL_E_OK);
  TEST_CHECK(Deque_Back(&Deque) != NULL);
  TEST_CHECK(*(const uint32_t *)Deque_Back(&Deque) == B);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_PushFront_NullDeque(TestContextType * Context)
{
  uint32_t Value = 10U;

  TEST_CHECK(Deque_PushFront(NULL, &Value) == NCL_E_NULL_PTR);
}

static void Test_Deque_PushFront_NullValue(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_PushFront(&Deque, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}


static void Test_Deque_PushFront_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushFront(&Deque, &A) == NCL_E_OK);
  TEST_CHECK(Deque_Size(&Deque) == 1U);
  TEST_CHECK(*(uint32_t *)Deque_Front(&Deque) == A);
  TEST_CHECK(*(uint32_t *)Deque_Back(&Deque) == A);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_PushFront_NonEmpty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;
  uint32_t B = 20U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushFront(&Deque, &B) == NCL_E_OK);
  TEST_CHECK(Deque_Size(&Deque) == 2U);
  TEST_CHECK(*(uint32_t *)Deque_Front(&Deque) == B);
  TEST_CHECK(*(uint32_t *)Deque_Back(&Deque) == A);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_PushBack_NullDeque(TestContextType * Context)
{
  uint32_t Value = 10U;

  TEST_CHECK(Deque_PushBack(NULL, &Value) == NCL_E_NULL_PTR);
}

static void Test_Deque_PushBack_NullValue(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_PushBack(&Deque, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}


static void Test_Deque_PushBack_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_CHECK(Deque_Size(&Deque) == 1U);
  TEST_CHECK(*(uint32_t *)Deque_Front(&Deque) == A);
  TEST_CHECK(*(uint32_t *)Deque_Back(&Deque) == A);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_PushBack_NonEmpty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;
  uint32_t B = 20U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushBack(&Deque, &B) == NCL_E_OK);
  TEST_CHECK(Deque_Size(&Deque) == 2U);
  TEST_CHECK(*(uint32_t *)Deque_Front(&Deque) == A);
  TEST_CHECK(*(uint32_t *)Deque_Back(&Deque) == B);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_PopFront_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Deque_PopFront(NULL) == NCL_E_NULL_PTR);
}

static void Test_Deque_PopFront_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_PopFront(&Deque) == NCL_E_EMPTY);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_PopFront_Single(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_CHECK(Deque_PopFront(&Deque) == NCL_E_OK);
  TEST_CHECK(Deque_IsEmpty(&Deque));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_PopFront_Multiple(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;
  uint32_t B = 20U;
  uint32_t C = 30U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushBack(&Deque, &B) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushBack(&Deque, &C) == NCL_E_OK);
  TEST_CHECK(Deque_PopFront(&Deque) == NCL_E_OK);
  TEST_CHECK(Deque_Size(&Deque) == 2U);
  TEST_CHECK(*(uint32_t *)Deque_Front(&Deque) == B);
  TEST_CHECK(*(uint32_t *)Deque_Back(&Deque) == C);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_PopBack_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Deque_PopBack(NULL) == NCL_E_NULL_PTR);
}

static void Test_Deque_PopBack_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_PopBack(&Deque) == NCL_E_EMPTY);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_PopBack_Single(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_CHECK(Deque_PopBack(&Deque) == NCL_E_OK);
  TEST_CHECK(Deque_IsEmpty(&Deque));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_PopBack_Multiple(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;
  uint32_t B = 20U;
  uint32_t C = 30U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushBack(&Deque, &B) == NCL_E_OK);
  TEST_REQUIRE(Deque_PushBack(&Deque, &C) == NCL_E_OK);
  TEST_CHECK(Deque_PopBack(&Deque) == NCL_E_OK);
  TEST_CHECK(Deque_Size(&Deque) == 2U);
  TEST_CHECK(*(uint32_t *)Deque_Front(&Deque) == A);
  TEST_CHECK(*(uint32_t *)Deque_Back(&Deque) == B);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}


static void Test_Deque_Edge(TestContextType * Context)
{
  TEST_RUN(Test_Deque_FrontConst_NullPtr);
  TEST_RUN(Test_Deque_FrontConst_Empty);
  TEST_RUN(Test_Deque_FrontConst_NonEmpty);
  TEST_RUN(Test_Deque_Front_NullPtr);
  TEST_RUN(Test_Deque_Front_Empty);
  TEST_RUN(Test_Deque_Front_NonEmpty);
  TEST_RUN(Test_Deque_BackConst_NullPtr);
  TEST_RUN(Test_Deque_BackConst_Empty);
  TEST_RUN(Test_Deque_BackConst_NonEmpty);
  TEST_RUN(Test_Deque_Back_NullPtr);
  TEST_RUN(Test_Deque_Back_Empty);
  TEST_RUN(Test_Deque_Back_NonEmpty);
  TEST_RUN(Test_Deque_PushFront_NullDeque);
  TEST_RUN(Test_Deque_PushFront_NullValue);
  TEST_RUN(Test_Deque_PushFront_Empty);
  TEST_RUN(Test_Deque_PushFront_NonEmpty);
  TEST_RUN(Test_Deque_PushBack_NullDeque);
  TEST_RUN(Test_Deque_PushBack_NullValue);
  TEST_RUN(Test_Deque_PushBack_Empty);
  TEST_RUN(Test_Deque_PushBack_NonEmpty);
  TEST_RUN(Test_Deque_PopFront_NullPtr);
  TEST_RUN(Test_Deque_PopFront_Empty);
  TEST_RUN(Test_Deque_PopFront_Single);
  TEST_RUN(Test_Deque_PopFront_Multiple);
  TEST_RUN(Test_Deque_PopBack_NullPtr);
  TEST_RUN(Test_Deque_PopBack_Empty);
  TEST_RUN(Test_Deque_PopBack_Single);
  TEST_RUN(Test_Deque_PopBack_Multiple);
}

/**
 * Debug
 */

static NCLResultType Test_Deque_PrintData(const void * Data)
{
  (void)Data;
  return NCL_E_OK;
}

static void Test_Deque_Print_NullDeque(TestContextType * Context)
{
  TEST_CHECK(Deque_Print(NULL, Test_Deque_PrintData) == NCL_E_NULL_PTR);
}

static void Test_Deque_Print_NullCallback(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_Print(&Deque, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_Print_InvalidState(TestContextType * Context)
{
  DequeType Deque = { 0 };

  TEST_CHECK(Deque_Print(&Deque, Test_Deque_PrintData) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Deque_Print_Empty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Deque_Print(&Deque, Test_Deque_PrintData) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_Print_NonEmpty(TestContextType * Context)
{
  DequeType Deque = { 0 };
  bool Initialized = false;
  uint32_t A = 10U;

  TEST_REQUIRE(Deque_Init(&Deque, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Deque_PushBack(&Deque, &A) == NCL_E_OK);
  TEST_CHECK(Deque_Print(&Deque, Test_Deque_PrintData) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Deque_Deinit(&Deque) == NCL_E_OK);
  }
}

static void Test_Deque_Debug(TestContextType * Context)
{
  TEST_RUN(Test_Deque_Print_NullDeque);
  TEST_RUN(Test_Deque_Print_NullCallback);
  TEST_RUN(Test_Deque_Print_InvalidState);
  TEST_RUN(Test_Deque_Print_Empty);
  TEST_RUN(Test_Deque_Print_NonEmpty);
}

int main(void)
{
  TestContextType Context;

  TestUtils_Init(&Context);

  TestUtils_Run(&Context, Test_Deque_Lifecycle, "Deque Lifecycle");
  TestUtils_Run(&Context, Test_Deque_State, "Deque State Operations");
  TestUtils_Run(&Context, Test_Deque_Edge, "Deque Edge Operations");
  TestUtils_Run(&Context, Test_Deque_Debug, "Deque Debug");

  return TestUtils_Report(&Context);
}
