
/**
 * TestUtils.h
 *
 * Notes:
 *
 */

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
  size_t CheckCount;
  size_t FailedCheckCount;

  size_t TestCount;
  size_t FailedTestCount;

  size_t Depth;
} TestContextType;

typedef void (*TestFunctionType)(TestContextType * Context);

/**
 * Test Context
 */

void TestUtils_Init(TestContextType * Context);

/**
 * Test Operations
 */

void TestUtils_Check(TestContextType * Context, bool ExpressionResult, const char * Expression, const char * File, int Line);

void TestUtils_Run(TestContextType * Context, TestFunctionType Function, const char *Name);

int TestUtils_Report(const TestContextType *Context);

/**
 * Test Macros
 */

#define TEST_CHECK(Expression)                      \
  do                                                \
  {                                                 \
    bool ExpressionResult = (Expression);           \
                                                    \
    TestUtils_Check(                                \
      Context,                                      \
      ExpressionResult,                             \
      #Expression,                                  \
      __FILE__,                                     \
      __LINE__                                      \
    );                                              \
  } while (0)

#define TEST_REQUIRE(Expression)                    \
  do                                                \
  {                                                 \
    bool ExpressionResult = (Expression);           \
                                                    \
    TestUtils_Check(                                \
      Context,                                      \
      ExpressionResult,                             \
      #Expression,                                  \
      __FILE__,                                     \
      __LINE__                                      \
    );                                              \
                                                    \
    if (!ExpressionResult)                          \
    {                                               \
      goto Cleanup;                                 \
    }                                               \
  } while (0)

#define TEST_RUN(Function)                          \
  do                                                \
  {                                                 \
    TestUtils_Run(Context, (Function), #Function);  \
  } while (0)

#endif
