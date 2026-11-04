/**
 * HashTable.c
 */

#include "HashTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 3/4 = 0.75
#define HASHTABLE_LOAD_FACTOR_NUM 3U
#define HASHTABLE_LOAD_FACTOR_DEN 4U

/**
 * Helper Functions
 */

static uint32_t HashTable_DJB2(const char * Str)
{
  uint32_t Hash = 5381U;
  unsigned char c = 0U;

  if (Str == NULL)
  {
    // Do nothing.
  }
  else
  {
    while (*Str != '\0')
    {
      c = (unsigned char)*Str;
      Str = Str + 1;
      Hash = ((Hash << 5) + Hash) + (uint32_t)c; /* h * 33 + c */
    }
  }

  return Hash;
}

static const HashNodeType * HashTable_NodeInBucketConst(const HashTableType * HashTable, size_t BucketIndex, const char * Key, uint32_t Hash)
{
  const HashNodeType * Node = NULL;

  Node = HashTable->Buckets[BucketIndex];

  while ((Node != NULL) && !((Node->Hash == Hash) && (strcmp(Node->Key, Key) == 0)))
  {
    Node = Node->Next;
  }

  return Node;
}

static HashNodeType * HashTable_NodeInBucket(HashTableType * HashTable, size_t BucketIndex, const char * Key, uint32_t Hash)
{
  HashNodeType * Node = NULL;

  Node = HashTable->Buckets[BucketIndex];

  while ((Node != NULL) && !((Node->Hash == Hash) && (strcmp(Node->Key, Key) == 0)))
  {
    Node = Node->Next;
  }

  return Node;
}

static const HashNodeType * HashTable_NodeAtConst(const HashTableType * HashTable, const char * Key)
{
  const HashNodeType * Node = NULL;
  uint32_t Hash = 0U;
  size_t BucketIndex = 0U;

  Hash = HashTable_DJB2(Key);
  BucketIndex = Hash % HashTable->BucketCount;
  Node = HashTable_NodeInBucketConst(HashTable, BucketIndex, Key, Hash);

  return Node;
}

static HashNodeType * HashTable_NodeAt(HashTableType * HashTable, const char * Key)
{
  HashNodeType * Node = NULL;
  uint32_t Hash = 0U;
  size_t BucketIndex = 0U;

  Hash = HashTable_DJB2(Key);
  BucketIndex = Hash % HashTable->BucketCount;
  Node = HashTable_NodeInBucket(HashTable, BucketIndex, Key, Hash);

  return Node;
}

static size_t HashTable_Threshold(size_t BucketCount)
{
  size_t Result = 0U;
  size_t Num = HASHTABLE_LOAD_FACTOR_NUM;
  size_t Den = HASHTABLE_LOAD_FACTOR_DEN;

  // Result = BucketCount * Num / Den;
  Result = (BucketCount / Den) * Num + ((BucketCount % Den) * Num + Den - 1U) / Den;  // Safe

  return Result;
}

static bool HashTable_NeedsGrow(const HashTableType * HashTable)
{
  bool Result = false;
  size_t Threshold = 0U;

  if (HashTable == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (HashTable->BucketCount == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Threshold = HashTable_Threshold(HashTable->BucketCount);
    Result = (HashTable->Size >= Threshold);
  }

  return Result;
}

static NCLResultType HashTable_Rehash(HashTableType * HashTable, size_t BucketCount)
{
  NCLResultType Result = NCL_E_OK;
  HashNodeType ** Buckets = NULL;
  HashNodeType * Node = NULL;
  HashNodeType * Next = NULL;
  size_t BucketIndex = 0U;

  if (HashTable == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (HashTable->Buckets == NULL || HashTable->BucketCount == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (BucketCount == 0U || BucketCount > SIZE_MAX / sizeof(HashNodeType *))
  {
    Result = NCL_E_ALLOC_FAILED;
  }
  else if (BucketCount == HashTable->BucketCount)
  {
    // Do nothing.
  }
  else
  {
    Buckets = calloc(BucketCount, sizeof(HashNodeType *));

    if (Buckets == NULL)
    {
      Result = NCL_E_ALLOC_FAILED;
    }
    else
    {
      for (size_t i = 0U; i < HashTable->BucketCount; i++)
      {
        Node = HashTable->Buckets[i];

        while (Node != NULL)
        {
          Next = Node->Next;

          BucketIndex = Node->Hash % BucketCount;
          Node->Next = Buckets[BucketIndex];
          Buckets[BucketIndex] = Node;

          Node = Next;
        }
      }

      free(HashTable->Buckets);
      HashTable->Buckets = Buckets;
      HashTable->BucketCount = BucketCount;
    }
  }

  return Result;
}

/**
 * Lifecycle
 */

NCLResultType HashTable_Init(HashTableType * HashTable, size_t ElementSize, size_t BucketCount)
{
  NCLResultType Result = NCL_E_OK;
  HashNodeType ** Buckets = NULL;

  if (HashTable == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (ElementSize == 0U || ElementSize > SIZE_MAX - sizeof(HashNodeType))
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (BucketCount == 0U || BucketCount > SIZE_MAX / sizeof(HashNodeType *))
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    Buckets = calloc(BucketCount, sizeof(HashNodeType *));

    if (Buckets == NULL)
    {
      HashTable->Buckets = NULL;
      HashTable->ElementSize = 0U;
      HashTable->BucketCount = 0U;
      HashTable->Size = 0U;

      Result = NCL_E_ALLOC_FAILED;
    }
    else
    {
      HashTable->Buckets = Buckets;
      HashTable->ElementSize = ElementSize;
      HashTable->BucketCount = BucketCount;
      HashTable->Size = 0U;
    }
  }

  return Result;
}

NCLResultType HashTable_Deinit(HashTableType * HashTable)
{
  NCLResultType Result = NCL_E_OK;

  if (HashTable == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (HashTable->Buckets == NULL || HashTable->BucketCount == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    Result = HashTable_Clear(HashTable);

    free(HashTable->Buckets);
    HashTable->Buckets = NULL;
    HashTable->ElementSize = 0U;
    HashTable->Size = 0U;
    HashTable->BucketCount = 0U;
  }

  return Result;
}

/**
 * State Operations
 */

NCLResultType HashTable_Clear(HashTableType * HashTable)
{
  NCLResultType Result = NCL_E_OK;
  HashNodeType * Node = NULL;
  HashNodeType * Next = NULL;

  if (HashTable == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (HashTable->Buckets == NULL || HashTable->BucketCount == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    for (size_t i = 0U; i < HashTable->BucketCount; i++)
    {
      Node = HashTable->Buckets[i];

      while (Node != NULL)
      {
        Next = Node->Next;
        free(Node->Key);
        free(Node);
        Node = Next;
      }

      HashTable->Buckets[i] = NULL;
    }

    HashTable->Size = 0U;
  }

  return Result;
}

size_t HashTable_Size(const HashTableType * HashTable)
{
  size_t Result = 0U;

  if (HashTable == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (HashTable->Buckets == NULL || HashTable->BucketCount == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Result = HashTable->Size;
  }

  return Result;
}

bool HashTable_IsEmpty(const HashTableType * HashTable)
{
  bool Result = true;

  if (HashTable == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (HashTable->Buckets == NULL || HashTable->BucketCount == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Result = (HashTable->Size == 0U);
  }

  return Result;
}

bool HashTable_Contains(const HashTableType * HashTable, const char * Key)
{
  bool Result = false;

  if (HashTable == NULL || Key == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (HashTable->Buckets == NULL || HashTable->BucketCount == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Result = (HashTable_NodeAtConst(HashTable, Key) != NULL);
  }

  return Result;
}

/**
 * Iterators
 */

/**
 * Access
 */

const void * HashTable_DataAtConst(const HashTableType * HashTable, const char * Key)
{
  const void * Data = NULL;
  const HashNodeType * Node = NULL;

  if (HashTable == NULL || Key == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (HashTable->Buckets == NULL || HashTable->BucketCount == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Node = HashTable_NodeAtConst(HashTable, Key);

    if (Node == NULL)
    {
      // Do nothing.
    }
    else
    {
      Data = Node->Data;
    }
  }

  return Data;
}

void * HashTable_DataAt(HashTableType * HashTable, const char * Key)
{
  void * Data = NULL;
  HashNodeType * Node = NULL;

  if (HashTable == NULL || Key == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (HashTable->Buckets == NULL || HashTable->BucketCount == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Node = HashTable_NodeAt(HashTable, Key);

    if (Node == NULL)
    {
      // Do nothing.
    }
    else
    {
      Data = Node->Data;
    }
  }

  return Data;
}

NCLResultType HashTable_Get(const HashTableType * HashTable, const char * Key, void * Value)
{
  NCLResultType Result = NCL_E_OK;
  const HashNodeType * Node = NULL;

  if (HashTable == NULL || Key == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (HashTable->Buckets == NULL || HashTable->BucketCount == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    Node = HashTable_NodeAtConst(HashTable, Key);

    if (Node == NULL)
    {
      Result = NCL_E_NOT_FOUND;
    }
    else
    {
      memcpy(Value, Node->Data, HashTable->ElementSize);
    }
  }

  return Result;
}

NCLResultType HashTable_Set(HashTableType * HashTable, const char * Key, const void * Value)
{
  NCLResultType Result = NCL_E_OK;
  HashNodeType * Node = NULL;
  HashNodeType * NewNode = NULL;
  uint32_t Hash = 0U;
  size_t BucketIndex = 0U;

  if (HashTable == NULL || Key == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (HashTable->Buckets == NULL || HashTable->BucketCount == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    Hash = HashTable_DJB2(Key);
    BucketIndex = Hash % HashTable->BucketCount;
    Node = HashTable_NodeInBucket(HashTable, BucketIndex, Key, Hash);

    if (Node != NULL)
    {
      memcpy(Node->Data, Value, HashTable->ElementSize);
    }
    else
    {
      NewNode = malloc(sizeof(HashNodeType) + HashTable->ElementSize);

      if (NewNode == NULL)
      {
        Result = NCL_E_ALLOC_FAILED;
      }
      else
      {
        NewNode->Key = StrDup(Key);

        if (NewNode->Key == NULL)
        {
          free(NewNode);
          Result = NCL_E_ALLOC_FAILED;
        }
        else
        {
          memcpy(NewNode->Data, Value, HashTable->ElementSize);
          NewNode->Next = HashTable->Buckets[BucketIndex];
          NewNode->Hash = Hash;
          HashTable->Buckets[BucketIndex] = NewNode;
          HashTable->Size++;

          if (HashTable_NeedsGrow(HashTable))
          {
            NCLResultType RehashResult = NCL_E_OK;
            size_t BucketCount = 0U;

            if (HashTable->BucketCount > SIZE_MAX / 2U)
            {
              // Too large, skip growth.
              // Do nothing.
            }
            else
            {
              BucketCount = HashTable->BucketCount * 2U;
              RehashResult = HashTable_Rehash(HashTable, BucketCount);

              if (RehashResult == NCL_E_OK)
              {
                // Insertion succeeded; rehash succeeded.
                // Leave Result = NCL_E_OK.
              }
              else
              {
                // Do nothing.
              }
            }
          }
        }
      }
    }
  }

  return Result;
}

/**
 * Modification
 */

NCLResultType HashTable_Erase(HashTableType * HashTable, const char * Key)
{
  NCLResultType Result = NCL_E_OK;
  HashNodeType ** Link = NULL;
  HashNodeType * Node = NULL;
  uint32_t Hash = 0U;
  size_t BucketIndex = 0U;

  if (HashTable == NULL || Key == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (HashTable->Buckets == NULL || HashTable->BucketCount == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    Hash = HashTable_DJB2(Key);
    BucketIndex = Hash % HashTable->BucketCount;
    Link = &HashTable->Buckets[BucketIndex];

    while (*Link != NULL && !((*Link)->Hash == Hash && strcmp((*Link)->Key, Key) == 0))
    {
      Link = &(*Link)->Next;
    }

    if (*Link == NULL)
    {
      Result = NCL_E_NOT_FOUND;
    }
    else
    {
      Node = *Link;
      *Link = Node->Next;
      free(Node->Key);
      free(Node);
      HashTable->Size--;
    }
  }

  return Result;
}

/**
 * Debug
 */

NCLResultType HashTable_Print(const HashTableType * HashTable, PrintDataFunctionType PrintData)
{
  NCLResultType Result = NCL_E_OK;
  HashNodeType * Node = NULL;

  if (HashTable == NULL || PrintData == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (HashTable->ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (HashTable->Buckets == NULL || HashTable->BucketCount == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    printf("HashTable Data: \n");

    for (size_t i = 0U; i < HashTable->BucketCount; i++)
    {
      printf("[%zu]", i);

      Node = HashTable->Buckets[i];

      while (Node != NULL)
      {
        PrintData(Node->Data);
        Node = Node->Next;
      }

      printf("\n");
    }

    printf("HashTable->ElementSize  : %zu\n", HashTable->ElementSize);
    printf("HashTable->Size         : %zu\n", HashTable->Size);
    printf("HashTable->BucketCount  : %zu\n", HashTable->BucketCount);

    printf("\n");
  }

  return Result;
}
