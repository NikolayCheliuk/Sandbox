/**
 * TestUtilsTest.c
 * Verifies counters and propagation using an isolated context.
 */

#include "TestUtils.h"

static void Test_SamplePass(TestContextType * Context)
{
  TEST_CHECK(true);
  TEST_CHECK(2 + 2 == 4);
}

static void Test_SampleFail(TestContextType * Context)
{
  TEST_CHECK(false);
}

static void Test_TestUtils_Init(TestContextType * Context)
{
  TestContextType Local = { 99U, 99U, 99U, 99U, 99U };
  TestUtils_Init(&Local);
  TEST_CHECK(Local.CheckCount == 0U);
  TEST_CHECK(Local.FailedCheckCount == 0U);
  TEST_CHECK(Local.TestCount == 0U);
  TEST_CHECK(Local.FailedTestCount == 0U);
  TEST_CHECK(Local.Depth == 0U);
}

static void Test_TestUtils_Counts(TestContextType * Context)
{
  TestContextType Local;
  TestUtils_Init(&Local);
  TestUtils_Run(&Local, Test_SamplePass, "Pass");
  TEST_CHECK(Local.CheckCount == 2U);
  TEST_CHECK(Local.FailedCheckCount == 0U);
  TEST_CHECK(Local.TestCount == 1U);
  TEST_CHECK(Local.FailedTestCount == 0U);
  TEST_CHECK(Local.Depth == 0U);
  TEST_CHECK(TestUtils_Report(&Local) == 0);
}

static void Test_TestUtils_FailurePropagation(TestContextType * Context)
{
  TestContextType Local;
  TestUtils_Init(&Local);
  TestUtils_Run(&Local, Test_SampleFail, "ExpectedFailure");
  TEST_CHECK(Local.CheckCount == 1U);
  TEST_CHECK(Local.FailedCheckCount == 1U);
  TEST_CHECK(Local.TestCount == 1U);
  TEST_CHECK(Local.FailedTestCount == 1U);
  TEST_CHECK(TestUtils_Report(&Local) != 0);
}

static void Test_TestUtils_InvalidFunction(TestContextType * Context)
{
  TestContextType Local;
  TestUtils_Init(&Local);
  TestUtils_Run(&Local, NULL, "InvalidFunction");
  TEST_CHECK(Local.TestCount == 1U);
  TEST_CHECK(Local.FailedTestCount == 1U);
  TEST_CHECK(Local.FailedCheckCount == 0U);
}

static void Test_TestUtils(TestContextType * Context)
{
  TEST_RUN(Test_TestUtils_Init);
  TEST_RUN(Test_TestUtils_Counts);
  TEST_RUN(Test_TestUtils_FailurePropagation);
  TEST_RUN(Test_TestUtils_InvalidFunction);
}

int main(void)
{
  TestContextType State;
  TestContextType * Context = &State;
  TestUtils_Init(Context);
  TEST_RUN(Test_TestUtils);
  return TestUtils_Report(Context);
}
