/**
 * ListTest.c
 *
 * Regression tests for the public List API.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "List.h"
#include "TestUtils.h"

/**
 * Lifecycle
 */

static void Test_List_Init_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_Init(NULL, sizeof(uint32_t)) == NCL_E_NULL_PTR);
}

static void Test_List_Init_ZeroSize(TestContextType * Context)
{
  ListType List = { 0 };

  TEST_CHECK(List_Init(&List, 0U) == NCL_E_INVALID_ARGUMENT);
}

static void Test_List_Init_Overflow(TestContextType * Context)
{
  ListType List = { 0 };

  TEST_CHECK(List_Init(&List, SIZE_MAX) == NCL_E_INVALID_ARGUMENT);
}

static void Test_List_Init_Valid(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List.Size == 0U);
  TEST_CHECK(List.ElementSize == sizeof(uint32_t));
  TEST_CHECK(List.Sentinel.Next == &List.Sentinel);
  TEST_CHECK(List.Sentinel.Prev == &List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Deinit_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_Deinit(NULL) == NCL_E_NULL_PTR);
}

static void Test_List_Deinit_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_Deinit(&List) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(List.ElementSize == 0U);
  TEST_CHECK(List.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Deinit_Allocated(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_REQUIRE(List_Deinit(&List) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(List.ElementSize == 0U);
  TEST_CHECK(List.Size == 0U);
  TEST_CHECK(List.Sentinel.Next == &List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

/**
 * State Operations
 */

static void Test_List_Clear_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_Clear(NULL) == NCL_E_NULL_PTR);
}

static void Test_List_Clear_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Clear(&List) == NCL_E_OK);
  TEST_CHECK(List.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Clear_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_REQUIRE(List_PushBack(&List, &Other) == NCL_E_OK);
  TEST_CHECK(List_Clear(&List) == NCL_E_OK);
  TEST_CHECK(List.Size == 0U);
  TEST_CHECK(List.Sentinel.Next == &List.Sentinel);
  TEST_CHECK(List.Sentinel.Prev == &List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Size_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_Size(NULL) == 0U);
}

static void Test_List_Size_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Size(&List) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Size_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List_Size(&List) == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_IsEmpty_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_IsEmpty(NULL));
}

static void Test_List_IsEmpty_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_IsEmpty(&List));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_IsEmpty_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(!List_IsEmpty(&List));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

/**
 * Iterators
 */

static void Test_ListIterator_InitConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(ListIterator_InitConst(NULL) == NCL_E_NULL_PTR);
}

static void Test_ListIterator_InitConst_Valid(TestContextType * Context)
{
  ConstListIteratorType It = { 0 };

  TEST_CHECK(ListIterator_InitConst(&It) == NCL_E_OK);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_ListIterator_Init_NullPtr(TestContextType * Context)
{
  TEST_CHECK(ListIterator_Init(NULL) == NCL_E_NULL_PTR);
}

static void Test_ListIterator_Init_Valid(TestContextType * Context)
{
  ListIteratorType It = { 0 };

  TEST_CHECK(ListIterator_Init(&It) == NCL_E_OK);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_ListIterator_NextConst_NullList(TestContextType * Context)
{
  ConstListIteratorType It = { NULL, NULL };

  It = ListIterator_NextConst(It);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_ListIterator_NextConst_NullNode(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ConstListIteratorType It = { &List, NULL };

  It = ListIterator_NextConst(It);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_NextConst_AtEnd(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ConstListIteratorType It = List_EndConst(&List);

  It = ListIterator_NextConst(It);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_NextConst_Advance(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ConstListIteratorType It;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  It = List_BeginConst(&List);
  It = ListIterator_NextConst(It);
  TEST_CHECK(It.List == &List);
  TEST_CHECK(It.Node == &List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Next_NullList(TestContextType * Context)
{
  ListIteratorType It = { NULL, NULL };

  It = ListIterator_Next(It);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_ListIterator_Next_NullNode(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It = { &List, NULL };

  It = ListIterator_Next(It);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Next_AtEnd(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It = List_End(&List);

  It = ListIterator_Next(It);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Next_Advance(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  It = List_Begin(&List);
  It = ListIterator_Next(It);
  TEST_CHECK(It.List == &List);
  TEST_CHECK(It.Node == &List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_PrevConst_NullList(TestContextType * Context)
{
  ConstListIteratorType It = { NULL, NULL };

  It = ListIterator_PrevConst(It);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_ListIterator_PrevConst_NullNode(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ConstListIteratorType It = { &List, NULL };

  It = ListIterator_PrevConst(It);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_PrevConst_AtBegin(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ConstListIteratorType It;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  It = List_BeginConst(&List);
  It = ListIterator_PrevConst(It);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_PrevConst_Retreat(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ConstListIteratorType It;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  It = List_EndConst(&List);
  It = ListIterator_PrevConst(It);
  TEST_CHECK(It.List == &List);
  TEST_CHECK(It.Node == List.Sentinel.Next);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Prev_NullList(TestContextType * Context)
{
  ListIteratorType It = { NULL, NULL };

  It = ListIterator_Prev(It);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_ListIterator_Prev_NullNode(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It = { &List, NULL };

  It = ListIterator_Prev(It);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Prev_AtBegin(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  It = List_Begin(&List);
  It = ListIterator_Prev(It);
  TEST_CHECK(It.List == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Prev_Retreat(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  It = List_End(&List);
  It = ListIterator_Prev(It);
  TEST_CHECK(It.List == &List);
  TEST_CHECK(It.Node == List.Sentinel.Next);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_DataConst_NullList(TestContextType * Context)
{
  ConstListIteratorType It = { NULL, NULL };

  TEST_CHECK(ListIterator_DataConst(It) == NULL);
}

static void Test_ListIterator_DataConst_NullNode(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ConstListIteratorType It = { &List, NULL };

  TEST_CHECK(ListIterator_DataConst(It) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_DataConst_AtEnd(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(ListIterator_DataConst(List_EndConst(&List)) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_DataConst_Valid(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)ListIterator_DataConst(List_BeginConst(&List)) == 42U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Data_NullList(TestContextType * Context)
{
  ListIteratorType It = { NULL, NULL };

  TEST_CHECK(ListIterator_Data(It) == NULL);
}

static void Test_ListIterator_Data_NullNode(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It = { &List, NULL };

  TEST_CHECK(ListIterator_Data(It) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Data_AtEnd(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(ListIterator_Data(List_End(&List)) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Data_Valid(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)ListIterator_Data(List_Begin(&List)) == 42U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_EqualConst_NullListFirst(TestContextType * Context)
{
  ConstListIteratorType It1 = { NULL, NULL };
  ConstListIteratorType It2 = { NULL, NULL };

  TEST_CHECK(!ListIterator_EqualConst(It1, It2));
}

static void Test_ListIterator_EqualConst_NullListSecond(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ConstListIteratorType It1 = List_EndConst(&List);
  ConstListIteratorType It2 = { NULL, NULL };

  TEST_CHECK(!ListIterator_EqualConst(It1, It2));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_EqualConst_NullNodeFirst(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ConstListIteratorType It1 = { &List, NULL };
  ConstListIteratorType It2 = List_EndConst(&List);

  TEST_CHECK(!ListIterator_EqualConst(It1, It2));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_EqualConst_NullNodeSecond(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ConstListIteratorType It1 = List_EndConst(&List);
  ConstListIteratorType It2 = { &List, NULL };

  TEST_CHECK(!ListIterator_EqualConst(It1, It2));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_EqualConst_SameNode(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(ListIterator_EqualConst(List_EndConst(&List), List_EndConst(&List)));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_EqualConst_DifferentNode(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(!ListIterator_EqualConst(List_BeginConst(&List), List_EndConst(&List)));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_EqualConst_DifferentList(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListType Another;
  bool AnotherInitialized = false;

  TEST_REQUIRE(List_Init(&Another, sizeof(uint32_t)) == NCL_E_OK);
  AnotherInitialized = true;
  TEST_CHECK(!ListIterator_EqualConst(List_EndConst(&List), List_EndConst(&Another)));

  if (AnotherInitialized)
  {
    TEST_CHECK(List_Deinit(&Another) == NCL_E_OK);
  }

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Equal_NullListFirst(TestContextType * Context)
{
  ListIteratorType It1 = { NULL, NULL };
  ListIteratorType It2 = { NULL, NULL };

  TEST_CHECK(!ListIterator_Equal(It1, It2));
}

static void Test_ListIterator_Equal_NullListSecond(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It1 = List_End(&List);
  ListIteratorType It2 = { NULL, NULL };

  TEST_CHECK(!ListIterator_Equal(It1, It2));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Equal_NullNodeFirst(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It1 = { &List, NULL };
  ListIteratorType It2 = List_End(&List);

  TEST_CHECK(!ListIterator_Equal(It1, It2));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Equal_NullNodeSecond(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It1 = List_End(&List);
  ListIteratorType It2 = { &List, NULL };

  TEST_CHECK(!ListIterator_Equal(It1, It2));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Equal_SameNode(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(ListIterator_Equal(List_End(&List), List_End(&List)));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Equal_DifferentNode(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(!ListIterator_Equal(List_Begin(&List), List_End(&List)));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_ListIterator_Equal_DifferentList(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListType Another;
  bool AnotherInitialized = false;

  TEST_REQUIRE(List_Init(&Another, sizeof(uint32_t)) == NCL_E_OK);
  AnotherInitialized = true;
  TEST_CHECK(!ListIterator_Equal(List_End(&List), List_End(&Another)));

  if (AnotherInitialized)
  {
    TEST_CHECK(List_Deinit(&Another) == NCL_E_OK);
  }

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_BeginConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_BeginConst(NULL).List == NULL);
  TEST_CHECK(List_BeginConst(NULL).Node == NULL);
}

static void Test_List_BeginConst_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_BeginConst(&List).List == &List);
  TEST_CHECK(List_BeginConst(&List).Node == &List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_BeginConst_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List_BeginConst(&List).List == &List);
  TEST_CHECK(List_BeginConst(&List).Node == List.Sentinel.Next);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Begin_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_Begin(NULL).List == NULL);
  TEST_CHECK(List_Begin(NULL).Node == NULL);
}

static void Test_List_Begin_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Begin(&List).List == &List);
  TEST_CHECK(List_Begin(&List).Node == &List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Begin_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List_Begin(&List).List == &List);
  TEST_CHECK(List_Begin(&List).Node == List.Sentinel.Next);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_EndConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_EndConst(NULL).List == NULL);
  TEST_CHECK(List_EndConst(NULL).Node == NULL);
}

static void Test_List_EndConst_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_EndConst(&List).List == &List);
  TEST_CHECK(List_EndConst(&List).Node == &List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_EndConst_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List_EndConst(&List).List == &List);
  TEST_CHECK(List_EndConst(&List).Node == &List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_End_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_End(NULL).List == NULL);
  TEST_CHECK(List_End(NULL).Node == NULL);
}

static void Test_List_End_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_End(&List).List == &List);
  TEST_CHECK(List_End(&List).Node == &List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_End_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List_End(&List).List == &List);
  TEST_CHECK(List_End(&List).Node == &List.Sentinel);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

/**
 * Access
 */

static void Test_List_DataAtConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_DataAtConst(NULL, 0U) == NULL);
}

static void Test_List_DataAtConst_OutOfRange(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_DataAtConst(&List, 0U) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_DataAtConst_FromHead(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  for (uint32_t Index = 0U; Index < 4U; Index++)
  {
    TEST_REQUIRE(List_PushBack(&List, &Index) == NCL_E_OK);
  }

  TEST_CHECK(*(const uint32_t *)List_DataAtConst(&List, 0U) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_DataAtConst_FromTail(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  for (uint32_t Index = 0U; Index < 4U; Index++)
  {
    TEST_REQUIRE(List_PushBack(&List, &Index) == NCL_E_OK);
  }

  TEST_CHECK(*(const uint32_t *)List_DataAtConst(&List, 3U) == 3U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_DataAt_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_DataAt(NULL, 0U) == NULL);
}

static void Test_List_DataAt_OutOfRange(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_DataAt(&List, 0U) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_DataAt_FromHead(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  for (uint32_t Index = 0U; Index < 4U; Index++)
  {
    TEST_REQUIRE(List_PushBack(&List, &Index) == NCL_E_OK);
  }

  TEST_CHECK(*(const uint32_t *)List_DataAt(&List, 0U) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_DataAt_FromTail(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  for (uint32_t Index = 0U; Index < 4U; Index++)
  {
    TEST_REQUIRE(List_PushBack(&List, &Index) == NCL_E_OK);
  }

  TEST_CHECK(*(const uint32_t *)List_DataAt(&List, 3U) == 3U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Get_NullList(TestContextType * Context)
{
  TEST_CHECK(List_Get(NULL, 0U, NULL) == NCL_E_NULL_PTR);
}

static void Test_List_Get_NullValue(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Get(&List, 0U, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Get_OutOfRange(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Get(&List, 0U, &Result) == NCL_E_OUT_OF_RANGE);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Get_Valid(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List_Get(&List, 0U, &Result) == NCL_E_OK);
  TEST_CHECK(Result == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Set_NullList(TestContextType * Context)
{
  TEST_CHECK(List_Set(NULL, 0U, NULL) == NCL_E_NULL_PTR);
}

static void Test_List_Set_NullValue(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Set(&List, 0U, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Set_OutOfRange(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Set(&List, 0U, &Value) == NCL_E_OUT_OF_RANGE);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Set_Valid(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List_Set(&List, 0U, &Other) == NCL_E_OK);
  TEST_CHECK(*(uint32_t *)List_DataAt(&List, 0U) == Other);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

/**
 * Modification
 */

static void Test_List_Insert_NullList(TestContextType * Context)
{
  ListIteratorType It = { NULL, NULL };
  uint32_t Value = 42U;

  TEST_CHECK(List_Insert(NULL, It, &Value).List == NULL);
}

static void Test_List_Insert_NullValue(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Insert(&List, List_End(&List), NULL).List == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Insert_WrongList(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It = { NULL, NULL };

  TEST_CHECK(List_Insert(&List, It, &Value).List == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Insert_NullNode(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It = { &List, NULL };

  TEST_CHECK(List_Insert(&List, It, &Value).List == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Insert_BeforeEnd(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It = List_Insert(&List, List_End(&List), &Value);

  TEST_CHECK(It.List == &List);
  TEST_CHECK(It.Node == List.Sentinel.Next);
  TEST_CHECK(List.Size == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Insert_BeforeBegin(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  It = List_Insert(&List, List_Begin(&List), &Other);
  TEST_CHECK(It.List == &List);
  TEST_CHECK(List.Size == 2U);
  TEST_CHECK(*(uint32_t *)List_Front(&List) == Other);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Erase_NullList(TestContextType * Context)
{
  ListIteratorType It = { NULL, NULL };

  TEST_CHECK(List_Erase(NULL, It).List == NULL);
}

static void Test_List_Erase_WrongList(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It = { NULL, NULL };

  TEST_CHECK(List_Erase(&List, It).List == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Erase_NullNode(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It = { &List, NULL };

  TEST_CHECK(List_Erase(&List, It).List == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Erase_AtEnd(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Erase(&List, List_End(&List)).List == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Erase_Valid(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  ListIteratorType It;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  It = List_Erase(&List, List_Begin(&List));
  TEST_CHECK(It.List == &List);
  TEST_CHECK(It.Node == &List.Sentinel);
  TEST_CHECK(List.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

/**
 * Edge Operations
 */

static void Test_List_FrontConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_FrontConst(NULL) == NULL);
}

static void Test_List_FrontConst_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_FrontConst(&List) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_FrontConst_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_REQUIRE(List_PushBack(&List, &Other) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)List_FrontConst(&List) == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Front_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_Front(NULL) == NULL);
}

static void Test_List_Front_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Front(&List) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Front_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_REQUIRE(List_PushBack(&List, &Other) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)List_Front(&List) == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_BackConst_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_BackConst(NULL) == NULL);
}

static void Test_List_BackConst_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_BackConst(&List) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_BackConst_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_REQUIRE(List_PushBack(&List, &Other) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)List_BackConst(&List) == Other);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Back_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_Back(NULL) == NULL);
}

static void Test_List_Back_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Back(&List) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Back_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_REQUIRE(List_PushBack(&List, &Other) == NCL_E_OK);
  TEST_CHECK(*(const uint32_t *)List_Back(&List) == Other);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_PushFront_NullList(TestContextType * Context)
{
  TEST_CHECK(List_PushFront(NULL, NULL) == NCL_E_NULL_PTR);
}

static void Test_List_PushFront_NullValue(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_PushFront(&List, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_PushFront_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_PushFront(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List.Size == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_PushFront_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List_PushFront(&List, &Other) == NCL_E_OK);
  TEST_CHECK(*(uint32_t *)List_Front(&List) == Other);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_PushBack_NullList(TestContextType * Context)
{
  TEST_CHECK(List_PushBack(NULL, NULL) == NCL_E_NULL_PTR);
}

static void Test_List_PushBack_NullValue(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_PushBack(&List, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_PushBack_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List.Size == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_PushBack_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List_PushBack(&List, &Other) == NCL_E_OK);
  TEST_CHECK(*(uint32_t *)List_Back(&List) == Other);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_PopFront_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_PopFront(NULL) == NCL_E_NULL_PTR);
}

static void Test_List_PopFront_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_PopFront(&List) == NCL_E_EMPTY);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_PopFront_OneElement(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List_PopFront(&List) == NCL_E_OK);
  TEST_CHECK(List.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_PopFront_MultipleElements(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_REQUIRE(List_PushBack(&List, &Other) == NCL_E_OK);
  TEST_CHECK(List_PopFront(&List) == NCL_E_OK);
  TEST_CHECK(List.Size == 1U);
  TEST_CHECK(*(uint32_t *)List_Front(&List) == Other);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_PopBack_NullPtr(TestContextType * Context)
{
  TEST_CHECK(List_PopBack(NULL) == NCL_E_NULL_PTR);
}

static void Test_List_PopBack_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_PopBack(&List) == NCL_E_EMPTY);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_PopBack_OneElement(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List_PopBack(&List) == NCL_E_OK);
  TEST_CHECK(List.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_PopBack_MultipleElements(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_REQUIRE(List_PushBack(&List, &Other) == NCL_E_OK);
  TEST_CHECK(List_PopBack(&List) == NCL_E_OK);
  TEST_CHECK(List.Size == 1U);
  TEST_CHECK(*(uint32_t *)List_Front(&List) == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

/**
 * Debug
 */

static NCLResultType Test_List_PrintValue(const void * Data)
{
  (void)Data;

  return NCL_E_OK;
}

static void Test_List_Print_NullList(TestContextType * Context)
{
  TEST_CHECK(List_Print(NULL, Test_List_PrintValue) == NCL_E_NULL_PTR);
}

static void Test_List_Print_NullCallback(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Print(&List, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Print_InvalidArgument(TestContextType * Context)
{
  ListType List = { 0 };

  TEST_CHECK(List_Print(&List, Test_List_PrintValue) == NCL_E_INVALID_ARGUMENT);
}

static void Test_List_Print_Empty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(List_Print(&List, Test_List_PrintValue) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Print_NonEmpty(TestContextType * Context)
{
  ListType List;
  bool Initialized = false;
  uint32_t Value = 42U;
  uint32_t Other = 99U;
  uint32_t Result = 0U;
  (void)Other;
  (void)Result;
  (void)Value;

  TEST_REQUIRE(List_Init(&List, sizeof(uint32_t)) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(List_PushBack(&List, &Value) == NCL_E_OK);
  TEST_CHECK(List_Print(&List, Test_List_PrintValue) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(List_Deinit(&List) == NCL_E_OK);
  }
}

static void Test_List_Lifecycle(TestContextType * Context)
{
  TEST_RUN(Test_List_Init_NullPtr);
  TEST_RUN(Test_List_Init_ZeroSize);
  TEST_RUN(Test_List_Init_Overflow);
  TEST_RUN(Test_List_Init_Valid);
  TEST_RUN(Test_List_Deinit_NullPtr);
  TEST_RUN(Test_List_Deinit_Empty);
  TEST_RUN(Test_List_Deinit_Allocated);
}

static void Test_List_State(TestContextType * Context)
{
  TEST_RUN(Test_List_Clear_NullPtr);
  TEST_RUN(Test_List_Clear_Empty);
  TEST_RUN(Test_List_Clear_NonEmpty);
  TEST_RUN(Test_List_Size_NullPtr);
  TEST_RUN(Test_List_Size_Empty);
  TEST_RUN(Test_List_Size_NonEmpty);
  TEST_RUN(Test_List_IsEmpty_NullPtr);
  TEST_RUN(Test_List_IsEmpty_Empty);
  TEST_RUN(Test_List_IsEmpty_NonEmpty);
}

static void Test_List_Iterators(TestContextType * Context)
{
  TEST_RUN(Test_ListIterator_InitConst_NullPtr);
  TEST_RUN(Test_ListIterator_InitConst_Valid);
  TEST_RUN(Test_ListIterator_Init_NullPtr);
  TEST_RUN(Test_ListIterator_Init_Valid);
  TEST_RUN(Test_ListIterator_NextConst_NullList);
  TEST_RUN(Test_ListIterator_NextConst_NullNode);
  TEST_RUN(Test_ListIterator_NextConst_AtEnd);
  TEST_RUN(Test_ListIterator_NextConst_Advance);
  TEST_RUN(Test_ListIterator_Next_NullList);
  TEST_RUN(Test_ListIterator_Next_NullNode);
  TEST_RUN(Test_ListIterator_Next_AtEnd);
  TEST_RUN(Test_ListIterator_Next_Advance);
  TEST_RUN(Test_ListIterator_PrevConst_NullList);
  TEST_RUN(Test_ListIterator_PrevConst_NullNode);
  TEST_RUN(Test_ListIterator_PrevConst_AtBegin);
  TEST_RUN(Test_ListIterator_PrevConst_Retreat);
  TEST_RUN(Test_ListIterator_Prev_NullList);
  TEST_RUN(Test_ListIterator_Prev_NullNode);
  TEST_RUN(Test_ListIterator_Prev_AtBegin);
  TEST_RUN(Test_ListIterator_Prev_Retreat);
  TEST_RUN(Test_ListIterator_DataConst_NullList);
  TEST_RUN(Test_ListIterator_DataConst_NullNode);
  TEST_RUN(Test_ListIterator_DataConst_AtEnd);
  TEST_RUN(Test_ListIterator_DataConst_Valid);
  TEST_RUN(Test_ListIterator_Data_NullList);
  TEST_RUN(Test_ListIterator_Data_NullNode);
  TEST_RUN(Test_ListIterator_Data_AtEnd);
  TEST_RUN(Test_ListIterator_Data_Valid);
  TEST_RUN(Test_ListIterator_EqualConst_NullListFirst);
  TEST_RUN(Test_ListIterator_EqualConst_NullListSecond);
  TEST_RUN(Test_ListIterator_EqualConst_NullNodeFirst);
  TEST_RUN(Test_ListIterator_EqualConst_NullNodeSecond);
  TEST_RUN(Test_ListIterator_EqualConst_SameNode);
  TEST_RUN(Test_ListIterator_EqualConst_DifferentNode);
  TEST_RUN(Test_ListIterator_EqualConst_DifferentList);
  TEST_RUN(Test_ListIterator_Equal_NullListFirst);
  TEST_RUN(Test_ListIterator_Equal_NullListSecond);
  TEST_RUN(Test_ListIterator_Equal_NullNodeFirst);
  TEST_RUN(Test_ListIterator_Equal_NullNodeSecond);
  TEST_RUN(Test_ListIterator_Equal_SameNode);
  TEST_RUN(Test_ListIterator_Equal_DifferentNode);
  TEST_RUN(Test_ListIterator_Equal_DifferentList);
  TEST_RUN(Test_List_BeginConst_NullPtr);
  TEST_RUN(Test_List_BeginConst_Empty);
  TEST_RUN(Test_List_BeginConst_NonEmpty);
  TEST_RUN(Test_List_Begin_NullPtr);
  TEST_RUN(Test_List_Begin_Empty);
  TEST_RUN(Test_List_Begin_NonEmpty);
  TEST_RUN(Test_List_EndConst_NullPtr);
  TEST_RUN(Test_List_EndConst_Empty);
  TEST_RUN(Test_List_EndConst_NonEmpty);
  TEST_RUN(Test_List_End_NullPtr);
  TEST_RUN(Test_List_End_Empty);
  TEST_RUN(Test_List_End_NonEmpty);
}

static void Test_List_Access(TestContextType * Context)
{
  TEST_RUN(Test_List_DataAtConst_NullPtr);
  TEST_RUN(Test_List_DataAtConst_OutOfRange);
  TEST_RUN(Test_List_DataAtConst_FromHead);
  TEST_RUN(Test_List_DataAtConst_FromTail);
  TEST_RUN(Test_List_DataAt_NullPtr);
  TEST_RUN(Test_List_DataAt_OutOfRange);
  TEST_RUN(Test_List_DataAt_FromHead);
  TEST_RUN(Test_List_DataAt_FromTail);
  TEST_RUN(Test_List_Get_NullList);
  TEST_RUN(Test_List_Get_NullValue);
  TEST_RUN(Test_List_Get_OutOfRange);
  TEST_RUN(Test_List_Get_Valid);
  TEST_RUN(Test_List_Set_NullList);
  TEST_RUN(Test_List_Set_NullValue);
  TEST_RUN(Test_List_Set_OutOfRange);
  TEST_RUN(Test_List_Set_Valid);
}

static void Test_List_Modification(TestContextType * Context)
{
  TEST_RUN(Test_List_Insert_NullList);
  TEST_RUN(Test_List_Insert_NullValue);
  TEST_RUN(Test_List_Insert_WrongList);
  TEST_RUN(Test_List_Insert_NullNode);
  TEST_RUN(Test_List_Insert_BeforeEnd);
  TEST_RUN(Test_List_Insert_BeforeBegin);
  TEST_RUN(Test_List_Erase_NullList);
  TEST_RUN(Test_List_Erase_WrongList);
  TEST_RUN(Test_List_Erase_NullNode);
  TEST_RUN(Test_List_Erase_AtEnd);
  TEST_RUN(Test_List_Erase_Valid);
}

static void Test_List_Edge(TestContextType * Context)
{
  TEST_RUN(Test_List_FrontConst_NullPtr);
  TEST_RUN(Test_List_FrontConst_Empty);
  TEST_RUN(Test_List_FrontConst_NonEmpty);
  TEST_RUN(Test_List_Front_NullPtr);
  TEST_RUN(Test_List_Front_Empty);
  TEST_RUN(Test_List_Front_NonEmpty);
  TEST_RUN(Test_List_BackConst_NullPtr);
  TEST_RUN(Test_List_BackConst_Empty);
  TEST_RUN(Test_List_BackConst_NonEmpty);
  TEST_RUN(Test_List_Back_NullPtr);
  TEST_RUN(Test_List_Back_Empty);
  TEST_RUN(Test_List_Back_NonEmpty);
  TEST_RUN(Test_List_PushFront_NullList);
  TEST_RUN(Test_List_PushFront_NullValue);
  TEST_RUN(Test_List_PushFront_Empty);
  TEST_RUN(Test_List_PushFront_NonEmpty);
  TEST_RUN(Test_List_PushBack_NullList);
  TEST_RUN(Test_List_PushBack_NullValue);
  TEST_RUN(Test_List_PushBack_Empty);
  TEST_RUN(Test_List_PushBack_NonEmpty);
  TEST_RUN(Test_List_PopFront_NullPtr);
  TEST_RUN(Test_List_PopFront_Empty);
  TEST_RUN(Test_List_PopFront_OneElement);
  TEST_RUN(Test_List_PopFront_MultipleElements);
  TEST_RUN(Test_List_PopBack_NullPtr);
  TEST_RUN(Test_List_PopBack_Empty);
  TEST_RUN(Test_List_PopBack_OneElement);
  TEST_RUN(Test_List_PopBack_MultipleElements);
}

static void Test_List_Debug(TestContextType * Context)
{
  TEST_RUN(Test_List_Print_NullList);
  TEST_RUN(Test_List_Print_NullCallback);
  TEST_RUN(Test_List_Print_InvalidArgument);
  TEST_RUN(Test_List_Print_Empty);
  TEST_RUN(Test_List_Print_NonEmpty);
}

int main(void)
{
  TestContextType Context;

  TestUtils_Init(&Context);

  TestUtils_Run(&Context, Test_List_Lifecycle, "List Lifecycle");
  TestUtils_Run(&Context, Test_List_State, "List State");
  TestUtils_Run(&Context, Test_List_Iterators, "List Iterators");
  TestUtils_Run(&Context, Test_List_Access, "List Access");
  TestUtils_Run(&Context, Test_List_Modification, "List Modification");
  TestUtils_Run(&Context, Test_List_Edge, "List Edge");
  TestUtils_Run(&Context, Test_List_Debug, "List Debug");

  return TestUtils_Report(&Context);
}
