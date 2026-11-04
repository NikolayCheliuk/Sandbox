/**
 * UtilsTest.c
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "Utils.h"
#include "TestUtils.h"

static void Test_Utils_Compare(TestContextType * Context)
{
  int8_t A8 = -4;
  int8_t B8 = 5;
  int16_t A16 = -4;
  int16_t B16 = 5;
  int32_t A32 = -4;
  int32_t B32 = 5;
  int64_t A64 = -4;
  int64_t B64 = 5;
  uint8_t U8 = 4U;
  uint8_t V8 = 5U;
  uint16_t U16 = 4U;
  uint16_t V16 = 5U;
  uint32_t U32 = 4U;
  uint32_t V32 = 5U;
  uint64_t U64 = 4U;
  uint64_t V64 = 5U;
  TEST_CHECK(CompareInt8(&A8, &B8) < 0);
  TEST_CHECK(CompareInt16(&A16, &B16) < 0);
  TEST_CHECK(CompareInt32(&A32, &B32) < 0);
  TEST_CHECK(CompareInt64(&A64, &B64) < 0);
  TEST_CHECK(CompareUInt8(&U8, &V8) < 0);
  TEST_CHECK(CompareUInt16(&U16, &V16) < 0);
  TEST_CHECK(CompareUInt32(&U32, &V32) < 0);
  TEST_CHECK(CompareUInt64(&U64, &V64) < 0);
  TEST_CHECK(CompareInt32(&B32, &A32) > 0);
  TEST_CHECK(CompareInt32(&A32, &A32) == 0);
  TEST_CHECK(CompareUInt64(&V64, &U64) > 0);
  TEST_CHECK(CompareUInt64(&U64, &U64) == 0);
}

static void Test_Utils_Order(TestContextType * Context)
{
  int8_t A8 = -4, B8 = 5;
  int16_t A16 = -4, B16 = 5;
  int32_t A32 = -4, B32 = 5;
  int64_t A64 = -4, B64 = 5;
  uint8_t U8 = 4U, V8 = 5U;
  uint16_t U16 = 4U, V16 = 5U;
  uint32_t U32 = 4U, V32 = 5U;
  uint64_t U64 = 4U, V64 = 5U;
  TEST_CHECK(LessInt8(&A8, &B8));
  TEST_CHECK(LessInt16(&A16, &B16));
  TEST_CHECK(LessInt32(&A32, &B32));
  TEST_CHECK(LessInt64(&A64, &B64));
  TEST_CHECK(LessUInt8(&U8, &V8));
  TEST_CHECK(LessUInt16(&U16, &V16));
  TEST_CHECK(LessUInt32(&U32, &V32));
  TEST_CHECK(LessUInt64(&U64, &V64));
  TEST_CHECK(GreaterInt8(&B8, &A8));
  TEST_CHECK(GreaterInt16(&B16, &A16));
  TEST_CHECK(GreaterInt32(&B32, &A32));
  TEST_CHECK(GreaterInt64(&B64, &A64));
  TEST_CHECK(GreaterUInt8(&V8, &U8));
  TEST_CHECK(GreaterUInt16(&V16, &U16));
  TEST_CHECK(GreaterUInt32(&V32, &U32));
  TEST_CHECK(GreaterUInt64(&V64, &U64));
  TEST_CHECK(!LessInt32(&A32, &A32));
  TEST_CHECK(!GreaterUInt64(&U64, &U64));
}

static void Test_Utils_Swap(TestContextType * Context)
{
  uint32_t A = 10U;
  uint32_t B = 20U;
  Swap(&A, &B, sizeof(A));
  TEST_CHECK(A == 20U);
  TEST_CHECK(B == 10U);
  Swap(&A, &A, sizeof(A));
  TEST_CHECK(A == 20U);
}

static void Test_Utils_StrDup(TestContextType * Context)
{
  char Original[] = "hello";
  char * Copy = StrDup(Original);
  TEST_REQUIRE(Copy != NULL);
  TEST_CHECK(strcmp(Copy, Original) == 0);
  TEST_CHECK(Copy != Original);
  Original[0] = 'H';
  TEST_CHECK(strcmp(Copy, "hello") == 0);
Cleanup:
  free(Copy);
}

static void Test_Utils(TestContextType * Context)
{
  TEST_RUN(Test_Utils_Compare);
  TEST_RUN(Test_Utils_Order);
  TEST_RUN(Test_Utils_Swap);
  TEST_RUN(Test_Utils_StrDup);
}

int main(void)
{
  TestContextType State;
  TestContextType * Context = &State;
  TestUtils_Init(Context);
  TEST_RUN(Test_Utils);
  return TestUtils_Report(Context);
}
