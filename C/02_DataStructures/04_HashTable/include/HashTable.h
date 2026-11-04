/**
 * HashTable.h
 *
 * Notes:
 * - If Init fails, the hash-table is not considered initialized.
 *   Deinit must only be called on an initialized hash-table.
 * - After Deinit, only Init may be called.
 * - BucketCount is an initial number of buckets.
 * - Set(): In the case of an allocation error, the table remains unchanged, and the node is not recreated when updating an existing key.
 *   Consequently, pointers to the data remain valid even after a rehash.
 * - Print function is to be used for debug.
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <inttypes.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>

#include "Utils.h"

typedef struct HashNode
{
  struct HashNode * Next;
  char * Key;
  uint32_t Hash;
  _Alignas(long double) uint8_t Data[];
} HashNodeType;

typedef struct
{
  HashNodeType ** Buckets;
  size_t ElementSize;
  size_t BucketCount;
  size_t Size;
} HashTableType;

/**
 * Lifecycle
 */

NCLResultType HashTable_Init(HashTableType * HashTable, size_t ElementSize, size_t BucketCount);
NCLResultType HashTable_Deinit(HashTableType * HashTable);

/**
 * State Operations
 */

NCLResultType HashTable_Clear(HashTableType * HashTable);

size_t HashTable_Size(const HashTableType * HashTable);
bool HashTable_IsEmpty(const HashTableType * HashTable);
bool HashTable_Contains(const HashTableType * HashTable, const char * Key);

/**
 * Iterators
 */

/**
 * Access
 */

const void * HashTable_DataAtConst(const HashTableType * HashTable, const char * Key);
void * HashTable_DataAt(HashTableType * HashTable, const char * Key);

NCLResultType HashTable_Get(const HashTableType * HashTable, const char * Key, void * Value);
NCLResultType HashTable_Set(HashTableType * HashTable, const char * Key, const void * Value);

/**
 * Modification
 */

NCLResultType HashTable_Erase(HashTableType * HashTable, const char * Key);

/**
 * Edge Operations
 */

/**
 * Debug
 */

NCLResultType HashTable_Print(const HashTableType * HashTable, PrintDataFunctionType PrintData);

#endif // HASHTABLE_H
