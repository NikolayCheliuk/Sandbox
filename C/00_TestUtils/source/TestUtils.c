
/**
 * TestUtils.c
 *
 * Notes:
 *
 */

#include "TestUtils.h"

#include <stdio.h>

/**
 * Test Context
 */

void TestUtils_Init(TestContextType * Context)
{
  if (Context == NULL)
  {
    fprintf(stderr, "[ERROR] TestUtils_Init: Context == NULL\n");
  }
  else
  {
    *Context = (TestContextType){ 0 };
  }
}

/**
 * Test Operations
 */

void TestUtils_Check(TestContextType * Context, bool ExpressionResult, const char * Expression, const char * File, int Line)
{
  if (Context == NULL)
  {
    fprintf(stderr, "[ERROR] TestUtils_Check: Context == NULL\n");
  }
  else
  {
    ++Context->CheckCount;

    if ((Expression == NULL) || (File == NULL) || (Line <= 0))
    {
      ++Context->FailedCheckCount;

      fprintf(stderr, "[ERROR] TestUtils_Check: Invalid arguments\n");
    }
    else
    {
      if (!ExpressionResult)
      {
        ++Context->FailedCheckCount;

        fprintf(stderr, "[FAIL] %s:%d: %s\n", File, Line, Expression);
      }
    }
  }
}

void TestUtils_Run(TestContextType * Context, TestFunctionType Function, const char * Name)
{
  if (Context == NULL)
  {
    fprintf(stderr, "[ERROR] TestUtils_Run: Context == NULL\n");
  }
  else
  {
    TestContextType Before = *Context;

    ++Context->TestCount;

    if ((Function == NULL) || (Name == NULL))
    {
      ++Context->FailedTestCount;

      fprintf(stderr, "[ERROR] TestUtils_Run: %s == NULL\n", Function == NULL ? "Function" : "Name");

      printf("%*s[FAIL] %s\n", (int)(Before.Depth * 2U), "", Name != NULL ? Name : "<unnamed>");
    }
    else
    {
      ++Context->Depth;

      Function(Context);

      --Context->Depth;

      bool Failed = (Context->FailedCheckCount != Before.FailedCheckCount) || (Context->FailedTestCount != Before.FailedTestCount);

      if (Failed)
      {
        ++Context->FailedTestCount;
      }

      printf("%*s[%s] %s\n", (int)(Before.Depth * 2U), "", Failed ? "FAIL" : "PASS", Name);
    }
  }
}

int TestUtils_Report(const TestContextType * Context)
{
  int Result = 1;

  if (Context == NULL)
  {
    fprintf(stderr, "[ERROR] TestUtils_Report: Context == NULL\n");
  }
  else
  {
    printf("\nTests : %zu, failed: %zu\n"
             "Checks: %zu, failed: %zu\n",
      Context->TestCount,
      Context->FailedTestCount,
      Context->CheckCount,
      Context->FailedCheckCount
    );

    if ((Context->FailedCheckCount == 0U) && (Context->FailedTestCount == 0U))
    {
      Result = 0;
    }
  }

  return Result;
}
