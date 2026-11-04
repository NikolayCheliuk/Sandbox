/**
 * StackTest.c
 *
 * Regression tests for the public Stack API.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "Stack.h"
#include "TestUtils.h"

/**
 * Lifecycle
 */

static void Test_Stack_Init_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Stack_Init(NULL, sizeof(uint32_t)) == NCL_E_NULL_PTR);
}

static void Test_Stack_Init_InvalidArgument(TestContextType * Context)
{
  StackType Stack = { 0 };

  TEST_CHECK(Stack_Init(&Stack, 0U) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Stack_Init_Valid(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Stack.Vector.Data == NULL);
  TEST_CHECK(Stack.Vector.ElementSize == sizeof(uint32_t));
  TEST_CHECK(Stack_Size(&Stack) == 0U);
  TEST_CHECK(Stack_IsEmpty(&Stack));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Deinit_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Stack_Deinit(NULL) == NCL_E_NULL_PTR);
}

static void Test_Stack_Deinit_Empty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(Stack_Deinit(&Stack) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(Stack.Vector.Data == NULL);
  TEST_CHECK(Stack.Vector.ElementSize == 0U);
  TEST_CHECK(Stack.Vector.Size == 0U);
  TEST_CHECK(Stack.Vector.Capacity == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Deinit_Allocated(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 17U;
  TEST_REQUIRE(Stack_Push(&Stack, &Value) == NCL_E_OK);
  TEST_REQUIRE(Stack_Deinit(&Stack) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(Stack.Vector.Data == NULL);
  TEST_CHECK(Stack.Vector.ElementSize == 0U);
  TEST_CHECK(Stack.Vector.Size == 0U);
  TEST_CHECK(Stack.Vector.Capacity == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

/**
 * State Operations
 */

static void Test_Stack_Clear_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Stack_Clear(NULL) == NCL_E_NULL_PTR);
}

static void Test_Stack_Clear_InvalidState(TestContextType * Context)
{
  StackType Stack = { 0 };

  TEST_CHECK(Stack_Clear(&Stack) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Stack_Clear_Empty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Stack_Clear(&Stack) == NCL_E_OK);
  TEST_CHECK(Stack_Size(&Stack) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Clear_NonEmpty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 7U;
  TEST_REQUIRE(Stack_Push(&Stack, &Value) == NCL_E_OK);
  TEST_CHECK(Stack_Clear(&Stack) == NCL_E_OK);
  TEST_CHECK(Stack_Size(&Stack) == 0U);
  TEST_CHECK(Stack_IsEmpty(&Stack));
  TEST_CHECK(Stack.Vector.Capacity > 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Size_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Stack_Size(NULL) == 0U);
}

static void Test_Stack_Size_Empty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Stack_Size(&Stack) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Size_NonEmpty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 7U;
  TEST_REQUIRE(Stack_Push(&Stack, &Value) == NCL_E_OK);
  TEST_CHECK(Stack_Size(&Stack) == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_IsEmpty_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Stack_IsEmpty(NULL));
}

static void Test_Stack_IsEmpty_Empty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Stack_IsEmpty(&Stack));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_IsEmpty_NonEmpty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 7U;
  TEST_REQUIRE(Stack_Push(&Stack, &Value) == NCL_E_OK);
  TEST_CHECK(!Stack_IsEmpty(&Stack));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

/**
 * Edge Operations
 */

static void Test_Stack_TopConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Stack_TopConst(NULL) == NULL);
}

static void Test_Stack_TopConst_Empty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Stack_TopConst(&Stack) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_TopConst_NonEmpty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 7U;
  TEST_REQUIRE(Stack_Push(&Stack, &Value) == NCL_E_OK);
  const uint32_t * Top = Stack_TopConst(&Stack);
  TEST_REQUIRE(Top != NULL);
  TEST_CHECK(*Top == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Top_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Stack_Top(NULL) == NULL);
}

static void Test_Stack_Top_Empty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Stack_Top(&Stack) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Top_NonEmpty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 7U;
  TEST_REQUIRE(Stack_Push(&Stack, &Value) == NCL_E_OK);
  uint32_t * Top = Stack_Top(&Stack);
  TEST_REQUIRE(Top != NULL);
  TEST_CHECK(*Top == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Push_NullStack(TestContextType * Context)
{
  TEST_CHECK(Stack_Push(NULL, &(uint32_t){ 7U }) == NCL_E_NULL_PTR);
}

static void Test_Stack_Push_NullValue(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Stack_Push(&Stack, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Push_InvalidState(TestContextType * Context)
{
  StackType Stack = { 0 };
  uint32_t Value = 7U;

  TEST_CHECK(Stack_Push(&Stack, &Value) == NCL_E_NOT_OK);
}

static void Test_Stack_Push_Valid(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  uint32_t First = 7U;
  uint32_t Second = 8U;
  TEST_REQUIRE(Stack_Push(&Stack, &First) == NCL_E_OK);
  TEST_REQUIRE(Stack_Push(&Stack, &Second) == NCL_E_OK);
  TEST_CHECK(Stack_Size(&Stack) == 2U);
  const uint32_t * Top = Stack_TopConst(&Stack);
  TEST_REQUIRE(Top != NULL);
  TEST_CHECK(*Top == Second);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Pop_NullPtr(TestContextType * Context)
{
  TEST_CHECK(Stack_Pop(NULL) == NCL_E_NULL_PTR);
}

static void Test_Stack_Pop_InvalidState(TestContextType * Context)
{
  StackType Stack = { 0 };

  TEST_CHECK(Stack_Pop(&Stack) == NCL_E_EMPTY);
}

static void Test_Stack_Pop_Empty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Stack_Pop(&Stack) == NCL_E_EMPTY);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Pop_NonEmpty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  uint32_t First = 7U;
  uint32_t Second = 8U;
  TEST_REQUIRE(Stack_Push(&Stack, &First) == NCL_E_OK);
  TEST_REQUIRE(Stack_Push(&Stack, &Second) == NCL_E_OK);
  TEST_CHECK(Stack_Pop(&Stack) == NCL_E_OK);
  TEST_CHECK(Stack_Size(&Stack) == 1U);
  const uint32_t * Top = Stack_TopConst(&Stack);
  TEST_REQUIRE(Top != NULL);
  TEST_CHECK(*Top == First);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

/**
 * Debug
 */

static void Test_Stack_Print_NullStack(TestContextType * Context)
{
  TEST_CHECK(Stack_Print(NULL, PrintUInt32) == NCL_E_NULL_PTR);
}

static void Test_Stack_Print_NullCallback(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Stack_Print(&Stack, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Print_InvalidState(TestContextType * Context)
{
  StackType Stack = { 0 };

  TEST_CHECK(Stack_Print(&Stack, PrintUInt32) == NCL_E_INVALID_ARGUMENT);
}

static void Test_Stack_Print_Empty(TestContextType * Context)
{
  StackType Stack = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(Stack_Init(&Stack, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(Stack_Print(&Stack, PrintUInt32) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(Stack_Deinit(&Stack) == NCL_E_OK);
  }
}

static void Test_Stack_Lifecycle(TestContextType * Context)
{
  TEST_RUN(Test_Stack_Init_NullPtr);
  TEST_RUN(Test_Stack_Init_InvalidArgument);
  TEST_RUN(Test_Stack_Init_Valid);
  TEST_RUN(Test_Stack_Deinit_NullPtr);
  TEST_RUN(Test_Stack_Deinit_Empty);
  TEST_RUN(Test_Stack_Deinit_Allocated);
}

static void Test_Stack_State(TestContextType * Context)
{
  TEST_RUN(Test_Stack_Clear_NullPtr);
  TEST_RUN(Test_Stack_Clear_InvalidState);
  TEST_RUN(Test_Stack_Clear_Empty);
  TEST_RUN(Test_Stack_Clear_NonEmpty);
  TEST_RUN(Test_Stack_Size_NullPtr);
  TEST_RUN(Test_Stack_Size_Empty);
  TEST_RUN(Test_Stack_Size_NonEmpty);
  TEST_RUN(Test_Stack_IsEmpty_NullPtr);
  TEST_RUN(Test_Stack_IsEmpty_Empty);
  TEST_RUN(Test_Stack_IsEmpty_NonEmpty);
}

static void Test_Stack_Edge(TestContextType * Context)
{
  TEST_RUN(Test_Stack_TopConst_NullPtr);
  TEST_RUN(Test_Stack_TopConst_Empty);
  TEST_RUN(Test_Stack_TopConst_NonEmpty);
  TEST_RUN(Test_Stack_Top_NullPtr);
  TEST_RUN(Test_Stack_Top_Empty);
  TEST_RUN(Test_Stack_Top_NonEmpty);
  TEST_RUN(Test_Stack_Push_NullStack);
  TEST_RUN(Test_Stack_Push_NullValue);
  TEST_RUN(Test_Stack_Push_InvalidState);
  TEST_RUN(Test_Stack_Push_Valid);
  TEST_RUN(Test_Stack_Pop_NullPtr);
  TEST_RUN(Test_Stack_Pop_InvalidState);
  TEST_RUN(Test_Stack_Pop_Empty);
  TEST_RUN(Test_Stack_Pop_NonEmpty);
}

static void Test_Stack_Debug(TestContextType * Context)
{
  TEST_RUN(Test_Stack_Print_NullStack);
  TEST_RUN(Test_Stack_Print_NullCallback);
  TEST_RUN(Test_Stack_Print_InvalidState);
  TEST_RUN(Test_Stack_Print_Empty);
}

int main(void)
{
  TestContextType Context;

  TestUtils_Init(&Context);

  TestUtils_Run(&Context, Test_Stack_Lifecycle, "Stack Lifecycle");
  TestUtils_Run(&Context, Test_Stack_State, "Stack State");
  TestUtils_Run(&Context, Test_Stack_Edge, "Stack Edge");
  TestUtils_Run(&Context, Test_Stack_Debug, "Stack Debug");

  return TestUtils_Report(&Context);
}
