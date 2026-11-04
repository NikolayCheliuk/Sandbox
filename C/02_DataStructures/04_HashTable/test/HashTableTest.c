/**
 * HashTableTest.c
 *
 * Regression tests for the public HashTable API.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "HashTable.h"
#include "TestUtils.h"

static NCLResultType Test_HashTable_PrintValue(const void * Value)
{
  (void)Value;
  return NCL_E_OK;
}

/**
 * Lifecycle
 */

static void Test_HashTable_Init_NullPtr(TestContextType * Context)
{
  TEST_CHECK(HashTable_Init(NULL, sizeof(uint32_t), 4U) == NCL_E_NULL_PTR);
}

static void Test_HashTable_Init_ZeroElementSize(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(HashTable_Init(&HashTable, 0U, 4U) == NCL_E_INVALID_ARGUMENT);
  TEST_CHECK(HashTable.Buckets == NULL);
}

static void Test_HashTable_Init_ElementSizeOverflow(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(HashTable_Init(&HashTable, SIZE_MAX, 4U) == NCL_E_INVALID_ARGUMENT);
  TEST_CHECK(HashTable.Buckets == NULL);
}

static void Test_HashTable_Init_ZeroBucketCount(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(HashTable_Init(&HashTable, sizeof(uint32_t), 0U) == NCL_E_INVALID_ARGUMENT);
  TEST_CHECK(HashTable.Buckets == NULL);
}

static void Test_HashTable_Init_BucketCountOverflow(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(HashTable_Init(&HashTable, sizeof(uint32_t), SIZE_MAX) == NCL_E_INVALID_ARGUMENT);
  TEST_CHECK(HashTable.Buckets == NULL);
}

static void Test_HashTable_Init_Valid(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable.Buckets != NULL);
  TEST_CHECK(HashTable.ElementSize == sizeof(uint32_t));
  TEST_CHECK(HashTable.BucketCount == 4U);
  TEST_CHECK(HashTable.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Deinit_NullPtr(TestContextType * Context)
{
  TEST_CHECK(HashTable_Deinit(NULL) == NCL_E_NULL_PTR);
}

static void Test_HashTable_Deinit_InvalidState(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_NOT_OK);
}

static void Test_HashTable_Deinit_Empty(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Deinit(&HashTable) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(HashTable.Buckets == NULL);
  TEST_CHECK(HashTable.ElementSize == 0U);
  TEST_CHECK(HashTable.BucketCount == 0U);
  TEST_CHECK(HashTable.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Deinit_Populated(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 12U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_REQUIRE(HashTable_Deinit(&HashTable) == NCL_E_OK);
  Initialized = false;
  TEST_CHECK(HashTable.Buckets == NULL);
  TEST_CHECK(HashTable.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

/**
 * State Operations
 */

static void Test_HashTable_Clear_NullPtr(TestContextType * Context)
{
  TEST_CHECK(HashTable_Clear(NULL) == NCL_E_NULL_PTR);
}

static void Test_HashTable_Clear_InvalidState(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(HashTable_Clear(&HashTable) == NCL_E_NOT_OK);
}

static void Test_HashTable_Clear_Empty(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_Clear(&HashTable) == NCL_E_OK);
  TEST_CHECK(HashTable.Size == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Clear_Populated(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 12U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_REQUIRE(HashTable_Set(&HashTable, "b", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable_Clear(&HashTable) == NCL_E_OK);
  TEST_CHECK(HashTable.Size == 0U);
  TEST_CHECK(HashTable_DataAt(&HashTable, "a") == NULL);
  TEST_CHECK(HashTable_DataAt(&HashTable, "b") == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Size_NullPtr(TestContextType * Context)
{
  TEST_CHECK(HashTable_Size(NULL) == 0U);
}

static void Test_HashTable_Size_InvalidState(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(HashTable_Size(&HashTable) == 0U);
}

static void Test_HashTable_Size_Empty(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_Size(&HashTable) == 0U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Size_Populated(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 12U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable_Size(&HashTable) == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_IsEmpty_NullPtr(TestContextType * Context)
{
  TEST_CHECK(HashTable_IsEmpty(NULL));
}

static void Test_HashTable_IsEmpty_InvalidState(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(HashTable_IsEmpty(&HashTable));
}

static void Test_HashTable_IsEmpty_Empty(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_IsEmpty(&HashTable));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_IsEmpty_Populated(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 12U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_CHECK(!HashTable_IsEmpty(&HashTable));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Contains_NullTable(TestContextType * Context)
{
  TEST_CHECK(!HashTable_Contains(NULL, "a"));
}

static void Test_HashTable_Contains_NullKey(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(!HashTable_Contains(&HashTable, NULL));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Contains_InvalidState(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(!HashTable_Contains(&HashTable, "a"));
}

static void Test_HashTable_Contains_Missing(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(!HashTable_Contains(&HashTable, "a"));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Contains_Present(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 12U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable_Contains(&HashTable, "a"));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

/**
 * Access
 */

static void Test_HashTable_DataAtConst_NullTable(TestContextType * Context)
{
  TEST_CHECK(HashTable_DataAtConst(NULL, "a") == NULL);
}

static void Test_HashTable_DataAtConst_NullKey(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_DataAtConst(&HashTable, NULL) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_DataAtConst_InvalidState(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(HashTable_DataAtConst(&HashTable, "a") == NULL);
}

static void Test_HashTable_DataAtConst_Missing(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_DataAtConst(&HashTable, "a") == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_DataAtConst_Present(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 12U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable_DataAtConst(&HashTable, "a") != NULL);
  TEST_CHECK(*(const uint32_t *)HashTable_DataAtConst(&HashTable, "a") == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_DataAt_NullTable(TestContextType * Context)
{
  TEST_CHECK(HashTable_DataAt(NULL, "a") == NULL);
}

static void Test_HashTable_DataAt_NullKey(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_DataAt(&HashTable, NULL) == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_DataAt_InvalidState(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(HashTable_DataAt(&HashTable, "a") == NULL);
}

static void Test_HashTable_DataAt_Missing(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_DataAt(&HashTable, "a") == NULL);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_DataAt_Present(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 12U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable_DataAt(&HashTable, "a") != NULL);
  TEST_CHECK(*(const uint32_t *)HashTable_DataAt(&HashTable, "a") == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Get_NullTable(TestContextType * Context)
{
  uint32_t Value = 0U;

  TEST_CHECK(HashTable_Get(NULL, "a", &Value) == NCL_E_NULL_PTR);
}

static void Test_HashTable_Get_NullKey(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;
  uint32_t Value = 0U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_Get(&HashTable, NULL, &Value) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Get_NullValue(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_Get(&HashTable, "a", NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Get_InvalidState(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  uint32_t Value = 0U;

  TEST_CHECK(HashTable_Get(&HashTable, "a", &Value) == NCL_E_NOT_OK);
}

static void Test_HashTable_Get_Missing(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 99U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_Get(&HashTable, "missing", &Value) == NCL_E_NOT_FOUND);
  TEST_CHECK(Value == 99U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Get_Present(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 42U;
  uint32_t Output = 0U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable_Get(&HashTable, "a", &Output) == NCL_E_OK);
  TEST_CHECK(Output == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Set_NullTable(TestContextType * Context)
{
  uint32_t Value = 42U;

  TEST_CHECK(HashTable_Set(NULL, "a", &Value) == NCL_E_NULL_PTR);
}

static void Test_HashTable_Set_NullKey(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;
  uint32_t Value = 42U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_Set(&HashTable, NULL, &Value) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Set_NullValue(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_Set(&HashTable, "a", NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Set_InvalidState(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  uint32_t Value = 42U;

  TEST_CHECK(HashTable_Set(&HashTable, "a", &Value) == NCL_E_NOT_OK);
}

static void Test_HashTable_Set_Insert(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 42U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable.Size == 1U);
  TEST_CHECK(*(uint32_t *)HashTable_DataAt(&HashTable, "a") == Value);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Set_Update(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 42U;
  uint32_t Updated = 73U;
  void * Data = NULL;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  Data = HashTable_DataAt(&HashTable, "a");
  TEST_REQUIRE(Data != NULL);
  TEST_CHECK(HashTable_Set(&HashTable, "a", &Updated) == NCL_E_OK);
  TEST_CHECK(HashTable_DataAt(&HashTable, "a") == Data);
  TEST_CHECK(*(uint32_t *)Data == Updated);
  TEST_CHECK(HashTable.Size == 1U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Set_NoRehash(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 42U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable.BucketCount == 4U);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Set_Rehash(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 42U;
  void * Data = NULL;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  Data = HashTable_DataAt(&HashTable, "a");
  TEST_REQUIRE(Data != NULL);
  TEST_REQUIRE(HashTable_Set(&HashTable, "b", &Value) == NCL_E_OK);
  TEST_REQUIRE(HashTable_Set(&HashTable, "c", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable.BucketCount == 8U);
  TEST_CHECK(HashTable.Size == 3U);
  TEST_CHECK(HashTable_DataAt(&HashTable, "a") == Data);
  TEST_CHECK(HashTable_Contains(&HashTable, "b"));
  TEST_CHECK(HashTable_Contains(&HashTable, "c"));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Set_Collision(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 42U;
  uint32_t Other = 73U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_REQUIRE(HashTable_Set(&HashTable, "e", &Other) == NCL_E_OK);
  TEST_CHECK(HashTable.Size == 2U);
  TEST_CHECK(*(uint32_t *)HashTable_DataAt(&HashTable, "a") == Value);
  TEST_CHECK(*(uint32_t *)HashTable_DataAt(&HashTable, "e") == Other);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

/**
 * Modification
 */

static void Test_HashTable_Erase_NullTable(TestContextType * Context)
{
  TEST_CHECK(HashTable_Erase(NULL, "a") == NCL_E_NULL_PTR);
}

static void Test_HashTable_Erase_NullKey(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_Erase(&HashTable, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Erase_InvalidState(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(HashTable_Erase(&HashTable, "a") == NCL_E_NOT_OK);
}

static void Test_HashTable_Erase_Missing(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_Erase(&HashTable, "a") == NCL_E_NOT_FOUND);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Erase_Present(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 42U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable_Erase(&HashTable, "a") == NCL_E_OK);
  TEST_CHECK(HashTable.Size == 0U);
  TEST_CHECK(!HashTable_Contains(&HashTable, "a"));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Erase_CollisionHead(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 42U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_REQUIRE(HashTable_Set(&HashTable, "e", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable_Erase(&HashTable, "e") == NCL_E_OK);
  TEST_CHECK(HashTable_Contains(&HashTable, "a"));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Erase_CollisionTail(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 42U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_REQUIRE(HashTable_Set(&HashTable, "e", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable_Erase(&HashTable, "a") == NCL_E_OK);
  TEST_CHECK(HashTable_Contains(&HashTable, "e"));

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

/**
 * Debug
 */

static void Test_HashTable_Print_NullTable(TestContextType * Context)
{
  TEST_CHECK(HashTable_Print(NULL, Test_HashTable_PrintValue) == NCL_E_NULL_PTR);
}

static void Test_HashTable_Print_NullCallback(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_Print(&HashTable, NULL) == NCL_E_NULL_PTR);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Print_InvalidElementSize(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  TEST_CHECK(HashTable_Print(&HashTable, Test_HashTable_PrintValue) == NCL_E_INVALID_ARGUMENT);
}

static void Test_HashTable_Print_InvalidBuckets(TestContextType * Context)
{
  HashTableType HashTable = { 0 };

  HashTable.ElementSize = sizeof(uint32_t);
  TEST_CHECK(HashTable_Print(&HashTable, Test_HashTable_PrintValue) == NCL_E_NOT_OK);
}

static void Test_HashTable_Print_Empty(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_CHECK(HashTable_Print(&HashTable, Test_HashTable_PrintValue) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Print_Populated(TestContextType * Context)
{
  HashTableType HashTable = { 0 };
  bool Initialized = false;

  uint32_t Value = 42U;

  TEST_REQUIRE(HashTable_Init(&HashTable, sizeof(uint32_t), 4U) == NCL_E_OK);
  Initialized = true;

  TEST_REQUIRE(HashTable_Set(&HashTable, "a", &Value) == NCL_E_OK);
  TEST_CHECK(HashTable_Print(&HashTable, Test_HashTable_PrintValue) == NCL_E_OK);

Cleanup:

  if (Initialized)
  {
    TEST_CHECK(HashTable_Deinit(&HashTable) == NCL_E_OK);
  }
}

static void Test_HashTable_Lifecycle(TestContextType * Context)
{
  TEST_RUN(Test_HashTable_Init_NullPtr);
  TEST_RUN(Test_HashTable_Init_ZeroElementSize);
  TEST_RUN(Test_HashTable_Init_ElementSizeOverflow);
  TEST_RUN(Test_HashTable_Init_ZeroBucketCount);
  TEST_RUN(Test_HashTable_Init_BucketCountOverflow);
  TEST_RUN(Test_HashTable_Init_Valid);
  TEST_RUN(Test_HashTable_Deinit_NullPtr);
  TEST_RUN(Test_HashTable_Deinit_InvalidState);
  TEST_RUN(Test_HashTable_Deinit_Empty);
  TEST_RUN(Test_HashTable_Deinit_Populated);
}

static void Test_HashTable_State(TestContextType * Context)
{
  TEST_RUN(Test_HashTable_Clear_NullPtr);
  TEST_RUN(Test_HashTable_Clear_InvalidState);
  TEST_RUN(Test_HashTable_Clear_Empty);
  TEST_RUN(Test_HashTable_Clear_Populated);
  TEST_RUN(Test_HashTable_Size_NullPtr);
  TEST_RUN(Test_HashTable_Size_InvalidState);
  TEST_RUN(Test_HashTable_Size_Empty);
  TEST_RUN(Test_HashTable_Size_Populated);
  TEST_RUN(Test_HashTable_IsEmpty_NullPtr);
  TEST_RUN(Test_HashTable_IsEmpty_InvalidState);
  TEST_RUN(Test_HashTable_IsEmpty_Empty);
  TEST_RUN(Test_HashTable_IsEmpty_Populated);
  TEST_RUN(Test_HashTable_Contains_NullTable);
  TEST_RUN(Test_HashTable_Contains_NullKey);
  TEST_RUN(Test_HashTable_Contains_InvalidState);
  TEST_RUN(Test_HashTable_Contains_Missing);
  TEST_RUN(Test_HashTable_Contains_Present);
}

static void Test_HashTable_Access(TestContextType * Context)
{
  TEST_RUN(Test_HashTable_DataAtConst_NullTable);
  TEST_RUN(Test_HashTable_DataAtConst_NullKey);
  TEST_RUN(Test_HashTable_DataAtConst_InvalidState);
  TEST_RUN(Test_HashTable_DataAtConst_Missing);
  TEST_RUN(Test_HashTable_DataAtConst_Present);
  TEST_RUN(Test_HashTable_DataAt_NullTable);
  TEST_RUN(Test_HashTable_DataAt_NullKey);
  TEST_RUN(Test_HashTable_DataAt_InvalidState);
  TEST_RUN(Test_HashTable_DataAt_Missing);
  TEST_RUN(Test_HashTable_DataAt_Present);
  TEST_RUN(Test_HashTable_Get_NullTable);
  TEST_RUN(Test_HashTable_Get_NullKey);
  TEST_RUN(Test_HashTable_Get_NullValue);
  TEST_RUN(Test_HashTable_Get_InvalidState);
  TEST_RUN(Test_HashTable_Get_Missing);
  TEST_RUN(Test_HashTable_Get_Present);
  TEST_RUN(Test_HashTable_Set_NullTable);
  TEST_RUN(Test_HashTable_Set_NullKey);
  TEST_RUN(Test_HashTable_Set_NullValue);
  TEST_RUN(Test_HashTable_Set_InvalidState);
  TEST_RUN(Test_HashTable_Set_Insert);
  TEST_RUN(Test_HashTable_Set_Update);
  TEST_RUN(Test_HashTable_Set_NoRehash);
  TEST_RUN(Test_HashTable_Set_Rehash);
  TEST_RUN(Test_HashTable_Set_Collision);
}

static void Test_HashTable_Modification(TestContextType * Context)
{
  TEST_RUN(Test_HashTable_Erase_NullTable);
  TEST_RUN(Test_HashTable_Erase_NullKey);
  TEST_RUN(Test_HashTable_Erase_InvalidState);
  TEST_RUN(Test_HashTable_Erase_Missing);
  TEST_RUN(Test_HashTable_Erase_Present);
  TEST_RUN(Test_HashTable_Erase_CollisionHead);
  TEST_RUN(Test_HashTable_Erase_CollisionTail);
}

static void Test_HashTable_Debug(TestContextType * Context)
{
  TEST_RUN(Test_HashTable_Print_NullTable);
  TEST_RUN(Test_HashTable_Print_NullCallback);
  TEST_RUN(Test_HashTable_Print_InvalidElementSize);
  TEST_RUN(Test_HashTable_Print_InvalidBuckets);
  TEST_RUN(Test_HashTable_Print_Empty);
  TEST_RUN(Test_HashTable_Print_Populated);
}

int main(void)
{
  TestContextType Context;

  TestUtils_Init(&Context);

  TestUtils_Run(&Context, Test_HashTable_Lifecycle, "HashTable Lifecycle");
  TestUtils_Run(&Context, Test_HashTable_State, "HashTable State Operations");
  TestUtils_Run(&Context, Test_HashTable_Access, "HashTable Access");
  TestUtils_Run(&Context, Test_HashTable_Modification, "HashTable Modification");
  TestUtils_Run(&Context, Test_HashTable_Debug, "HashTable Debug");

  return TestUtils_Report(&Context);
}
