/**
 * BSTTest.c
 *
 * Regression tests for the public BST API.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "BST.h"
#include "TestUtils.h"


/**
 * Lifecycle
 */


static void Test_BST_Init_NullBST(TestContextType * Context)
{

  TEST_CHECK(BST_Init(NULL, sizeof(uint32_t), CompareUInt32) == NCL_E_NULL_PTR);
}

static void Test_BST_Init_NullCompare(TestContextType * Context)
{

  BSTType BST = { 0 };

  TEST_CHECK(BST_Init(&BST, sizeof(uint32_t), NULL) == NCL_E_NULL_PTR);
}

static void Test_BST_Init_ZeroSize(TestContextType * Context)
{

  BSTType BST = { 0 };

  TEST_CHECK(BST_Init(&BST, 0U, CompareUInt32) == NCL_E_INVALID_ARGUMENT);
}

static void Test_BST_Init_Overflow(TestContextType * Context)
{

  BSTType BST = { 0 };

  TEST_CHECK(BST_Init(&BST, SIZE_MAX, CompareUInt32) == NCL_E_INVALID_ARGUMENT);
}

static void Test_BST_Init_Valid(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(BST.Root == NULL);
  TEST_CHECK(BST.Size == 0U);
  TEST_CHECK(BST.ElementSize == sizeof(uint32_t));
  TEST_CHECK(BST.CompareFunction == CompareUInt32);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Deinit_Null(TestContextType * Context)
{

  TEST_CHECK(BST_Deinit(NULL) == NCL_E_NULL_PTR);
}

static void Test_BST_Deinit_Empty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Deinit(&BST) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(BST.Root == NULL);
  TEST_CHECK(BST.Size == 0U);
  TEST_CHECK(BST.ElementSize == 0U);
  TEST_CHECK(BST.CompareFunction == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Deinit_NonEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t Value = 12U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &Value) == NCL_E_OK);
  TEST_REQUIRE(BST_Deinit(&BST) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(BST.Root == NULL);
  TEST_CHECK(BST.Size == 0U);
  TEST_CHECK(BST.ElementSize == 0U);
  TEST_CHECK(BST.CompareFunction == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Lifecycle(TestContextType * Context)
{
  TEST_RUN(Test_BST_Init_NullBST);
  TEST_RUN(Test_BST_Init_NullCompare);
  TEST_RUN(Test_BST_Init_ZeroSize);
  TEST_RUN(Test_BST_Init_Overflow);
  TEST_RUN(Test_BST_Init_Valid);
  TEST_RUN(Test_BST_Deinit_Null);
  TEST_RUN(Test_BST_Deinit_Empty);
  TEST_RUN(Test_BST_Deinit_NonEmpty);
}

/**
 * State Operations
 */


static void Test_BST_Clear_Null(TestContextType * Context)
{

  TEST_CHECK(BST_Clear(NULL) == NCL_E_NULL_PTR);
}

static void Test_BST_Clear_Empty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(BST_Clear(&BST) == NCL_E_OK);
  TEST_CHECK(BST.Root == NULL);
  TEST_CHECK(BST.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Clear_NonEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t A = 10U;
  uint32_t B = 5U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &A) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &B) == NCL_E_OK);
  TEST_CHECK(BST_Clear(&BST) == NCL_E_OK);
  TEST_CHECK(BST.Root == NULL);
  TEST_CHECK(BST.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Size_Null(TestContextType * Context)
{

  TEST_CHECK(BST_Size(NULL) == 0U);
}

static void Test_BST_Size_Empty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(BST_Size(&BST) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Size_NonEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t Value = 7U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &Value) == NCL_E_OK);
  TEST_CHECK(BST_Size(&BST) == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_IsEmpty_Null(TestContextType * Context)
{

  TEST_CHECK(BST_IsEmpty(NULL));
}

static void Test_BST_IsEmpty_Empty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(BST_IsEmpty(&BST));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_IsEmpty_NonEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t Value = 7U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &Value) == NCL_E_OK);
  TEST_CHECK(!BST_IsEmpty(&BST));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_State(TestContextType * Context)
{
  TEST_RUN(Test_BST_Clear_Null);
  TEST_RUN(Test_BST_Clear_Empty);
  TEST_RUN(Test_BST_Clear_NonEmpty);
  TEST_RUN(Test_BST_Size_Null);
  TEST_RUN(Test_BST_Size_Empty);
  TEST_RUN(Test_BST_Size_NonEmpty);
  TEST_RUN(Test_BST_IsEmpty_Null);
  TEST_RUN(Test_BST_IsEmpty_Empty);
  TEST_RUN(Test_BST_IsEmpty_NonEmpty);
}

/**
 * Iterators
 */


static void Test_BST_Iterator_InitConst_Null(TestContextType * Context)
{

  TEST_CHECK(BSTIterator_InitConst(NULL) == NCL_E_NULL_PTR);
}

static void Test_BST_Iterator_InitConst_Valid(TestContextType * Context)
{

  ConstBSTIteratorType It = { 0 };

  TEST_CHECK(BSTIterator_InitConst(&It) == NCL_E_OK);
  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_BST_Iterator_Init_Null(TestContextType * Context)
{

  TEST_CHECK(BSTIterator_Init(NULL) == NCL_E_NULL_PTR);
}

static void Test_BST_Iterator_Init_Valid(TestContextType * Context)
{

  BSTIteratorType It = { 0 };

  TEST_CHECK(BSTIterator_Init(&It) == NCL_E_OK);
  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_BST_Iterator_NextConst_Invalid(TestContextType * Context)
{

  ConstBSTIteratorType It = { 0 };

  It = BSTIterator_NextConst(It);
  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_BST_Iterator_NextConst_End(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It = BST_EndConst(&BST);

  It = BSTIterator_NextConst(It);
  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_NextConst_RightChild(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V15 = 15U;
  uint32_t V12 = 12U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V12) == NCL_E_OK);
  It = BST_BeginConst(&BST);
  It = BSTIterator_NextConst(It);
  It = BSTIterator_NextConst(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_DataConst(It) == 15U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_NextConst_Parent(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  It = BST_BeginConst(&BST);
  It = BSTIterator_NextConst(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_DataConst(It) == 10U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_NextConst_Climb(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V7 = 7U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V7) == NCL_E_OK);
  It = BST_BeginConst(&BST);
  It = BSTIterator_NextConst(It);
  It = BSTIterator_NextConst(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_DataConst(It) == 10U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_NextConst_PastLast(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V15 = 15U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  It = BST_BeginConst(&BST);
  It = BSTIterator_NextConst(BSTIterator_NextConst(It));
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Next_Invalid(TestContextType * Context)
{

  BSTIteratorType It = { 0 };

  It = BSTIterator_Next(It);
  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_BST_Iterator_Next_End(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It = BST_End(&BST);

  It = BSTIterator_Next(It);
  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Next_RightChild(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V15 = 15U;
  uint32_t V12 = 12U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V12) == NCL_E_OK);
  It = BST_Begin(&BST);
  It = BSTIterator_Next(It);
  It = BSTIterator_Next(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_Data(It) == 15U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Next_Parent(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  It = BST_Begin(&BST);
  It = BSTIterator_Next(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_Data(It) == 10U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Next_Climb(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V7 = 7U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V7) == NCL_E_OK);
  It = BST_Begin(&BST);
  It = BSTIterator_Next(It);
  It = BSTIterator_Next(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_Data(It) == 10U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Next_PastLast(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V15 = 15U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  It = BST_Begin(&BST);
  It = BSTIterator_Next(BSTIterator_Next(It));
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_PrevConst_Invalid(TestContextType * Context)
{

  ConstBSTIteratorType It = { 0 };

  It = BSTIterator_PrevConst(It);
  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_BST_Iterator_PrevConst_EndEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It = BST_EndConst(&BST);

  It = BSTIterator_PrevConst(It);
  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_PrevConst_EndNonEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V15 = 15U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  It = BST_EndConst(&BST);
  It = BSTIterator_PrevConst(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_DataConst(It) == 15U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_PrevConst_LeftChild(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V7 = 7U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V7) == NCL_E_OK);
  It = BST_EndConst(&BST);
  It = BST_BeginConst(&BST);
  It = BSTIterator_NextConst(It);
  It = BSTIterator_NextConst(It);
  It = BSTIterator_PrevConst(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_DataConst(It) == 7U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_PrevConst_Parent(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  It = BST_EndConst(&BST);
  It = BSTIterator_PrevConst(It);
  It = BSTIterator_PrevConst(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_DataConst(It) == 5U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_PrevConst_Climb(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V7 = 7U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V7) == NCL_E_OK);
  It = BST_EndConst(&BST);
  It = BSTIterator_PrevConst(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_DataConst(It) == 10U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_PrevConst_BeforeFirst(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  It = BST_BeginConst(&BST);
  It = BSTIterator_PrevConst(It);
  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Prev_Invalid(TestContextType * Context)
{

  BSTIteratorType It = { 0 };

  It = BSTIterator_Prev(It);
  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_BST_Iterator_Prev_EndEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It = BST_End(&BST);

  It = BSTIterator_Prev(It);
  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Prev_EndNonEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V15 = 15U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  It = BST_End(&BST);
  It = BSTIterator_Prev(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_Data(It) == 15U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Prev_LeftChild(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V7 = 7U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V7) == NCL_E_OK);
  It = BST_End(&BST);
  It = BST_Begin(&BST);
  It = BSTIterator_Next(It);
  It = BSTIterator_Next(It);
  It = BSTIterator_Prev(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_Data(It) == 7U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Prev_Parent(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  It = BST_End(&BST);
  It = BSTIterator_Prev(It);
  It = BSTIterator_Prev(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_Data(It) == 5U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Prev_Climb(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V7 = 7U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V7) == NCL_E_OK);
  It = BST_End(&BST);
  It = BSTIterator_Prev(It);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_Data(It) == 10U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Prev_BeforeFirst(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It;
  uint32_t V10 = 10U;
  uint32_t V5 = 5U;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  It = BST_Begin(&BST);
  It = BSTIterator_Prev(It);
  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_DataConst_Invalid(TestContextType * Context)
{

  ConstBSTIteratorType It = { 0 };

  TEST_CHECK(BSTIterator_DataConst(It) == NULL);
}

static void Test_BST_Iterator_DataConst_End(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It = BST_EndConst(&BST);

  TEST_CHECK(BSTIterator_DataConst(It) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_DataConst_Valid(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 19U;
  ConstBSTIteratorType It;

  TEST_REQUIRE(BST_Insert(&BST, &Value) == NCL_E_OK);
  It = BST_BeginConst(&BST);
  TEST_CHECK(BSTIterator_DataConst(It) != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_DataConst(It) == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Data_Invalid(TestContextType * Context)
{

  BSTIteratorType It = { 0 };

  TEST_CHECK(BSTIterator_Data(It) == NULL);
}

static void Test_BST_Iterator_Data_End(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It = BST_End(&BST);

  TEST_CHECK(BSTIterator_Data(It) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Data_Valid(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 19U;
  BSTIteratorType It;

  TEST_REQUIRE(BST_Insert(&BST, &Value) == NCL_E_OK);
  It = BST_Begin(&BST);
  TEST_CHECK(BSTIterator_Data(It) != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_Data(It) == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_EqualConst_InvalidFirst(TestContextType * Context)
{

  ConstBSTIteratorType First = { 0 };
  ConstBSTIteratorType Second = { 0 };

  TEST_CHECK(!BSTIterator_EqualConst(First, Second));
}

static void Test_BST_Iterator_EqualConst_InvalidSecond(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType First = BST_EndConst(&BST);
  ConstBSTIteratorType Second = { 0 };

  TEST_CHECK(!BSTIterator_EqualConst(First, Second));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_EqualConst_SameEnd(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType First = BST_EndConst(&BST);
  ConstBSTIteratorType Second = BST_EndConst(&BST);

  TEST_CHECK(BSTIterator_EqualConst(First, Second));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_EqualConst_DifferentNode(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 5U;
  ConstBSTIteratorType First;
  ConstBSTIteratorType Second;

  TEST_REQUIRE(BST_Insert(&BST, &Value) == NCL_E_OK);
  First = BST_BeginConst(&BST);
  Second = BST_EndConst(&BST);
  TEST_CHECK(!BSTIterator_EqualConst(First, Second));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_EqualConst_SameNode(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 5U;
  ConstBSTIteratorType First;
  ConstBSTIteratorType Second;

  TEST_REQUIRE(BST_Insert(&BST, &Value) == NCL_E_OK);
  First = BST_BeginConst(&BST);
  Second = BST_BeginConst(&BST);
  TEST_CHECK(BSTIterator_EqualConst(First, Second));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_EqualConst_DifferentBST(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTType Other = { 0 };
  ConstBSTIteratorType First = BST_EndConst(&BST);
  ConstBSTIteratorType Second;

  TEST_REQUIRE(BST_Init(&Other, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Second = BST_EndConst(&Other);
  TEST_CHECK(!BSTIterator_EqualConst(First, Second));
  TEST_CHECK(BST_Deinit(&Other) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Equal_InvalidFirst(TestContextType * Context)
{

  BSTIteratorType First = { 0 };
  BSTIteratorType Second = { 0 };

  TEST_CHECK(!BSTIterator_Equal(First, Second));
}

static void Test_BST_Iterator_Equal_InvalidSecond(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType First = BST_End(&BST);
  BSTIteratorType Second = { 0 };

  TEST_CHECK(!BSTIterator_Equal(First, Second));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Equal_SameEnd(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType First = BST_End(&BST);
  BSTIteratorType Second = BST_End(&BST);

  TEST_CHECK(BSTIterator_Equal(First, Second));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Equal_DifferentNode(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 5U;
  BSTIteratorType First;
  BSTIteratorType Second;

  TEST_REQUIRE(BST_Insert(&BST, &Value) == NCL_E_OK);
  First = BST_Begin(&BST);
  Second = BST_End(&BST);
  TEST_CHECK(!BSTIterator_Equal(First, Second));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Equal_SameNode(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  uint32_t Value = 5U;
  BSTIteratorType First;
  BSTIteratorType Second;

  TEST_REQUIRE(BST_Insert(&BST, &Value) == NCL_E_OK);
  First = BST_Begin(&BST);
  Second = BST_Begin(&BST);
  TEST_CHECK(BSTIterator_Equal(First, Second));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterator_Equal_DifferentBST(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTType Other = { 0 };
  BSTIteratorType First = BST_End(&BST);
  BSTIteratorType Second;

  TEST_REQUIRE(BST_Init(&Other, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Second = BST_End(&Other);
  TEST_CHECK(!BSTIterator_Equal(First, Second));
  TEST_CHECK(BST_Deinit(&Other) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_BeginConst_Null(TestContextType * Context)
{

  ConstBSTIteratorType It = BST_BeginConst(NULL);

  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_BST_BeginConst_Empty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It = BST_BeginConst(&BST);

  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_BeginConst_NonEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  uint32_t A = 10U;
  uint32_t B = 5U;
  ConstBSTIteratorType It;

  TEST_REQUIRE(BST_Insert(&BST, &A) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &B) == NCL_E_OK);
  It = BST_BeginConst(&BST);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_DataConst(It) == 5U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Begin_Null(TestContextType * Context)
{

  BSTIteratorType It = BST_Begin(NULL);

  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_BST_Begin_Empty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It = BST_Begin(&BST);

  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Begin_NonEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  uint32_t A = 10U;
  uint32_t B = 5U;
  BSTIteratorType It;

  TEST_REQUIRE(BST_Insert(&BST, &A) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &B) == NCL_E_OK);
  It = BST_Begin(&BST);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node != NULL);
  TEST_CHECK(*(const uint32_t *)BSTIterator_Data(It) == 5U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_EndConst_Null(TestContextType * Context)
{

  ConstBSTIteratorType It = BST_EndConst(NULL);

  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_BST_EndConst_Empty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  ConstBSTIteratorType It = BST_EndConst(&BST);

  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_EndConst_NonEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  uint32_t A = 10U;
  uint32_t B = 5U;
  ConstBSTIteratorType It;

  TEST_REQUIRE(BST_Insert(&BST, &A) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &B) == NCL_E_OK);
  It = BST_EndConst(&BST);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_End_Null(TestContextType * Context)
{

  BSTIteratorType It = BST_End(NULL);

  TEST_CHECK(It.BST == NULL);
  TEST_CHECK(It.Node == NULL);
}

static void Test_BST_End_Empty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BSTIteratorType It = BST_End(&BST);

  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_End_NonEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  uint32_t A = 10U;
  uint32_t B = 5U;
  BSTIteratorType It;

  TEST_REQUIRE(BST_Insert(&BST, &A) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &B) == NCL_E_OK);
  It = BST_End(&BST);
  TEST_CHECK(It.BST == &BST);
  TEST_CHECK(It.Node == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Iterators(TestContextType * Context)
{
  TEST_RUN(Test_BST_Iterator_InitConst_Null);
  TEST_RUN(Test_BST_Iterator_InitConst_Valid);
  TEST_RUN(Test_BST_Iterator_Init_Null);
  TEST_RUN(Test_BST_Iterator_Init_Valid);
  TEST_RUN(Test_BST_Iterator_NextConst_Invalid);
  TEST_RUN(Test_BST_Iterator_NextConst_End);
  TEST_RUN(Test_BST_Iterator_NextConst_RightChild);
  TEST_RUN(Test_BST_Iterator_NextConst_Parent);
  TEST_RUN(Test_BST_Iterator_NextConst_Climb);
  TEST_RUN(Test_BST_Iterator_NextConst_PastLast);
  TEST_RUN(Test_BST_Iterator_Next_Invalid);
  TEST_RUN(Test_BST_Iterator_Next_End);
  TEST_RUN(Test_BST_Iterator_Next_RightChild);
  TEST_RUN(Test_BST_Iterator_Next_Parent);
  TEST_RUN(Test_BST_Iterator_Next_Climb);
  TEST_RUN(Test_BST_Iterator_Next_PastLast);
  TEST_RUN(Test_BST_Iterator_PrevConst_Invalid);
  TEST_RUN(Test_BST_Iterator_PrevConst_EndEmpty);
  TEST_RUN(Test_BST_Iterator_PrevConst_EndNonEmpty);
  TEST_RUN(Test_BST_Iterator_PrevConst_LeftChild);
  TEST_RUN(Test_BST_Iterator_PrevConst_Parent);
  TEST_RUN(Test_BST_Iterator_PrevConst_Climb);
  TEST_RUN(Test_BST_Iterator_PrevConst_BeforeFirst);
  TEST_RUN(Test_BST_Iterator_Prev_Invalid);
  TEST_RUN(Test_BST_Iterator_Prev_EndEmpty);
  TEST_RUN(Test_BST_Iterator_Prev_EndNonEmpty);
  TEST_RUN(Test_BST_Iterator_Prev_LeftChild);
  TEST_RUN(Test_BST_Iterator_Prev_Parent);
  TEST_RUN(Test_BST_Iterator_Prev_Climb);
  TEST_RUN(Test_BST_Iterator_Prev_BeforeFirst);
  TEST_RUN(Test_BST_Iterator_DataConst_Invalid);
  TEST_RUN(Test_BST_Iterator_DataConst_End);
  TEST_RUN(Test_BST_Iterator_DataConst_Valid);
  TEST_RUN(Test_BST_Iterator_Data_Invalid);
  TEST_RUN(Test_BST_Iterator_Data_End);
  TEST_RUN(Test_BST_Iterator_Data_Valid);
  TEST_RUN(Test_BST_Iterator_EqualConst_InvalidFirst);
  TEST_RUN(Test_BST_Iterator_EqualConst_InvalidSecond);
  TEST_RUN(Test_BST_Iterator_EqualConst_SameEnd);
  TEST_RUN(Test_BST_Iterator_EqualConst_DifferentNode);
  TEST_RUN(Test_BST_Iterator_EqualConst_SameNode);
  TEST_RUN(Test_BST_Iterator_EqualConst_DifferentBST);
  TEST_RUN(Test_BST_Iterator_Equal_InvalidFirst);
  TEST_RUN(Test_BST_Iterator_Equal_InvalidSecond);
  TEST_RUN(Test_BST_Iterator_Equal_SameEnd);
  TEST_RUN(Test_BST_Iterator_Equal_DifferentNode);
  TEST_RUN(Test_BST_Iterator_Equal_SameNode);
  TEST_RUN(Test_BST_Iterator_Equal_DifferentBST);
  TEST_RUN(Test_BST_BeginConst_Null);
  TEST_RUN(Test_BST_BeginConst_Empty);
  TEST_RUN(Test_BST_BeginConst_NonEmpty);
  TEST_RUN(Test_BST_Begin_Null);
  TEST_RUN(Test_BST_Begin_Empty);
  TEST_RUN(Test_BST_Begin_NonEmpty);
  TEST_RUN(Test_BST_EndConst_Null);
  TEST_RUN(Test_BST_EndConst_Empty);
  TEST_RUN(Test_BST_EndConst_NonEmpty);
  TEST_RUN(Test_BST_End_Null);
  TEST_RUN(Test_BST_End_Empty);
  TEST_RUN(Test_BST_End_NonEmpty);
}

/**
 * Access
 */


static void Test_BST_Contains_NullBST(TestContextType * Context)
{

  uint32_t Value = 3U;

  TEST_CHECK(!BST_Contains(NULL, &Value));
}

static void Test_BST_Contains_NullValue(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(!BST_Contains(&BST, NULL));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Contains_InvalidCompare(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t Value = 3U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BST.CompareFunction = NULL;
  TEST_CHECK(!BST_Contains(&BST, &Value));
  BST.CompareFunction = CompareUInt32;

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Contains_Empty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t Value = 3U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(!BST_Contains(&BST, &Value));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Contains_Root(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t A = 10U;
  uint32_t B = 5U;
  uint32_t C = 15U;
  uint32_t Value = 10U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &A) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &B) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &C) == NCL_E_OK);
  TEST_CHECK(BST_Contains(&BST, &Value));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Contains_Left(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t A = 10U;
  uint32_t B = 5U;
  uint32_t C = 15U;
  uint32_t Value = 5U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &A) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &B) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &C) == NCL_E_OK);
  TEST_CHECK(BST_Contains(&BST, &Value));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Contains_Right(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t A = 10U;
  uint32_t B = 5U;
  uint32_t C = 15U;
  uint32_t Value = 15U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &A) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &B) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &C) == NCL_E_OK);
  TEST_CHECK(BST_Contains(&BST, &Value));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Contains_MissingLeft(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t A = 10U;
  uint32_t B = 5U;
  uint32_t C = 15U;
  uint32_t Value = 2U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &A) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &B) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &C) == NCL_E_OK);
  TEST_CHECK(!BST_Contains(&BST, &Value));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Contains_MissingRight(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t A = 10U;
  uint32_t B = 5U;
  uint32_t C = 15U;
  uint32_t Value = 20U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &A) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &B) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &C) == NCL_E_OK);
  TEST_CHECK(!BST_Contains(&BST, &Value));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Access(TestContextType * Context)
{
  TEST_RUN(Test_BST_Contains_NullBST);
  TEST_RUN(Test_BST_Contains_NullValue);
  TEST_RUN(Test_BST_Contains_InvalidCompare);
  TEST_RUN(Test_BST_Contains_Empty);
  TEST_RUN(Test_BST_Contains_Root);
  TEST_RUN(Test_BST_Contains_Left);
  TEST_RUN(Test_BST_Contains_Right);
  TEST_RUN(Test_BST_Contains_MissingLeft);
  TEST_RUN(Test_BST_Contains_MissingRight);
}

/**
 * Modification
 */


static void Test_BST_Insert_NullBST(TestContextType * Context)
{

  uint32_t Value = 1U;

  TEST_CHECK(BST_Insert(NULL, &Value) == NCL_E_NULL_PTR);
}

static void Test_BST_Insert_NullValue(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(BST_Insert(&BST, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Insert_InvalidCompare(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t Value = 1U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BST.CompareFunction = NULL;
  TEST_CHECK(BST_Insert(&BST, &Value) == NCL_E_INVALID_ARGUMENT);
  BST.CompareFunction = CompareUInt32;

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Insert_Root(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_CHECK(BST.Size == 1U);
  TEST_CHECK(BST_Contains(&BST, &V10));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Insert_Left(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t V5 = 5U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_CHECK(BST.Size == 2U);
  TEST_CHECK(BST_Contains(&BST, &V5));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Insert_Right(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t V15 = 15U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  TEST_CHECK(BST.Size == 2U);
  TEST_CHECK(BST_Contains(&BST, &V15));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Insert_LeftNested(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V7 = 7U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V7) == NCL_E_OK);
  TEST_CHECK(BST.Size == 3U);
  TEST_CHECK(BST_Contains(&BST, &V7));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Insert_RightNested(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t V15 = 15U;
  uint32_t V12 = 12U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V12) == NCL_E_OK);
  TEST_CHECK(BST.Size == 3U);
  TEST_CHECK(BST_Contains(&BST, &V12));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Insert_Duplicate(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t Value = 10U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &Value) == NCL_E_OK);
  TEST_CHECK(BST_Insert(&BST, &Value) == NCL_E_ALREADY_EXISTS);
  TEST_CHECK(BST.Size == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Erase_NullBST(TestContextType * Context)
{

  uint32_t Value = 1U;

  TEST_CHECK(BST_Erase(NULL, &Value) == NCL_E_NULL_PTR);
}

static void Test_BST_Erase_NullValue(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(BST_Erase(&BST, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Erase_InvalidCompare(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t Value = 1U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  BST.CompareFunction = NULL;
  TEST_CHECK(BST_Erase(&BST, &Value) == NCL_E_INVALID_ARGUMENT);
  BST.CompareFunction = CompareUInt32;

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Erase_Empty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t Value = 1U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(BST_Erase(&BST, &Value) == NCL_E_EMPTY);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Erase_NotFoundLeft(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V15 = 15U;
  uint32_t Value = 1U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  TEST_CHECK(BST_Erase(&BST, &Value) == NCL_E_NOT_FOUND);
  TEST_CHECK(BST.Size == 3U);
  TEST_CHECK(BST.Root != NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Erase_NotFoundRight(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V15 = 15U;
  uint32_t Value = 99U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  TEST_CHECK(BST_Erase(&BST, &Value) == NCL_E_NOT_FOUND);
  TEST_CHECK(BST.Size == 3U);
  TEST_CHECK(BST.Root != NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Erase_Leaf(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V15 = 15U;
  uint32_t Value = 5U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  TEST_CHECK(BST_Erase(&BST, &Value) == NCL_E_OK);
  TEST_CHECK(BST.Size == 2U);
  TEST_CHECK(!BST_Contains(&BST, &Value));
  TEST_CHECK(BST.Root != NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Erase_OneLeft(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V3 = 3U;
  uint32_t Value = 5U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V3) == NCL_E_OK);
  TEST_CHECK(BST_Erase(&BST, &Value) == NCL_E_OK);
  TEST_CHECK(BST.Size == 2U);
  TEST_CHECK(!BST_Contains(&BST, &Value));
  TEST_CHECK(BST.Root != NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Erase_OneRight(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t V15 = 15U;
  uint32_t V20 = 20U;
  uint32_t Value = 15U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V20) == NCL_E_OK);
  TEST_CHECK(BST_Erase(&BST, &Value) == NCL_E_OK);
  TEST_CHECK(BST.Size == 2U);
  TEST_CHECK(!BST_Contains(&BST, &Value));
  TEST_CHECK(BST.Root != NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Erase_TwoDirect(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V15 = 15U;
  uint32_t Value = 10U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  TEST_CHECK(BST_Erase(&BST, &Value) == NCL_E_OK);
  TEST_CHECK(BST.Size == 2U);
  TEST_CHECK(!BST_Contains(&BST, &Value));
  TEST_CHECK(BST.Root != NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Erase_TwoDeep(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V20 = 20U;
  uint32_t V15 = 15U;
  uint32_t Value = 10U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V20) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  TEST_CHECK(BST_Erase(&BST, &Value) == NCL_E_OK);
  TEST_CHECK(BST.Size == 3U);
  TEST_CHECK(!BST_Contains(&BST, &Value));
  TEST_CHECK(BST.Root != NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Erase_TwoDeepRight(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t V5 = 5U;
  uint32_t V20 = 20U;
  uint32_t V15 = 15U;
  uint32_t V17 = 17U;
  uint32_t Value = 10U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V5) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V20) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V15) == NCL_E_OK);
  TEST_REQUIRE(BST_Insert(&BST, &V17) == NCL_E_OK);
  TEST_CHECK(BST_Erase(&BST, &Value) == NCL_E_OK);
  TEST_CHECK(BST.Size == 4U);
  TEST_CHECK(!BST_Contains(&BST, &Value));
  TEST_CHECK(BST.Root != NULL);
  TEST_CHECK(BST_Contains(&BST, &V17));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Erase_RootOnly(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t V10 = 10U;
  uint32_t Value = 10U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &V10) == NCL_E_OK);
  TEST_CHECK(BST_Erase(&BST, &Value) == NCL_E_OK);
  TEST_CHECK(BST.Size == 0U);
  TEST_CHECK(!BST_Contains(&BST, &Value));
  TEST_CHECK(BST.Root == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Modification(TestContextType * Context)
{
  TEST_RUN(Test_BST_Insert_NullBST);
  TEST_RUN(Test_BST_Insert_NullValue);
  TEST_RUN(Test_BST_Insert_InvalidCompare);
  TEST_RUN(Test_BST_Insert_Root);
  TEST_RUN(Test_BST_Insert_Left);
  TEST_RUN(Test_BST_Insert_Right);
  TEST_RUN(Test_BST_Insert_LeftNested);
  TEST_RUN(Test_BST_Insert_RightNested);
  TEST_RUN(Test_BST_Insert_Duplicate);
  TEST_RUN(Test_BST_Erase_NullBST);
  TEST_RUN(Test_BST_Erase_NullValue);
  TEST_RUN(Test_BST_Erase_InvalidCompare);
  TEST_RUN(Test_BST_Erase_Empty);
  TEST_RUN(Test_BST_Erase_NotFoundLeft);
  TEST_RUN(Test_BST_Erase_NotFoundRight);
  TEST_RUN(Test_BST_Erase_Leaf);
  TEST_RUN(Test_BST_Erase_OneLeft);
  TEST_RUN(Test_BST_Erase_OneRight);
  TEST_RUN(Test_BST_Erase_TwoDirect);
  TEST_RUN(Test_BST_Erase_TwoDeep);
  TEST_RUN(Test_BST_Erase_TwoDeepRight);
  TEST_RUN(Test_BST_Erase_RootOnly);
}

/**
 * Debug
 */


static void Test_BST_Print_NullBST(TestContextType * Context)
{

  TEST_CHECK(BST_Print(NULL, PrintUInt32) == NCL_E_NULL_PTR);
}

static void Test_BST_Print_NullPrinter(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(BST_Print(&BST, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Print_Empty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(BST_Print(&BST, PrintUInt32) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Print_NonEmpty(TestContextType * Context)
{
  BSTType BST = { 0 };
  bool Initialized = false;

  uint32_t Value = 10U;

  TEST_REQUIRE(BST_Init(&BST, sizeof(uint32_t), CompareUInt32) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(BST_Insert(&BST, &Value) == NCL_E_OK);
  TEST_CHECK(BST_Print(&BST, PrintUInt32) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(BST_Deinit(&BST) == NCL_E_OK);
  }
}

static void Test_BST_Debug(TestContextType * Context)
{
  TEST_RUN(Test_BST_Print_NullBST);
  TEST_RUN(Test_BST_Print_NullPrinter);
  TEST_RUN(Test_BST_Print_Empty);
  TEST_RUN(Test_BST_Print_NonEmpty);
}

int main(void)
{
  TestContextType Context;

  TestUtils_Init(&Context);

  TestUtils_Run(&Context, Test_BST_Lifecycle, "BST Lifecycle");
  TestUtils_Run(&Context, Test_BST_State, "BST State");
  TestUtils_Run(&Context, Test_BST_Iterators, "BST Iterators");
  TestUtils_Run(&Context, Test_BST_Access, "BST Access");
  TestUtils_Run(&Context, Test_BST_Modification, "BST Modification");
  TestUtils_Run(&Context, Test_BST_Debug, "BST Debug");

  return TestUtils_Report(&Context);
}
