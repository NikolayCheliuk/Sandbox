/**
 * Utils.h
 *
 * Notes:
 * - Compare*, Greater* and Less* do not check pointers for NULL. Passing NULL is undefined behavior.
 */

#ifndef UTILS_H
#define UTILS_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "ErrorCodes.h"

typedef NCLResultType   (* ConstructFunctionType)     (void * This);
typedef NCLResultType   (* ConstructCopyFunctionType) (void * This, const void * That);
typedef NCLResultType   (* DestructFunctionType)      (void * This);

typedef NCLResultType   (* AssignFunctionType)        (void * This, const void * That);

typedef int32_t         (* CompareFunctionType)       (const void * AValue, const void * BValue);
typedef bool            (* OrderFunctionType)         (const void * AValue, const void * BValue);

typedef NCLResultType   (* PrintDataFunctionType)     (const void * Value);

typedef struct
{
  size_t Size;
} DataPropsType;

typedef struct
{
  ConstructFunctionType Construct;
  ConstructCopyFunctionType ConstructCopy;
  DestructFunctionType Destruct;

  AssignFunctionType Assign;
  CompareFunctionType Compare;
  OrderFunctionType Order;

  PrintDataFunctionType Print;
} DataOpsType;

typedef struct
{
  DataPropsType Props;
  DataOpsType Ops;
} DataType;

int CompareInt8(const void * AValue, const void * BValue);
int CompareInt16(const void * AValue, const void * BValue);
int CompareInt32(const void * AValue, const void * BValue);
int CompareInt64(const void * AValue, const void * BValue);

int CompareUInt8(const void * AValue, const void * BValue);
int CompareUInt16(const void * AValue, const void * BValue);
int CompareUInt32(const void * AValue, const void * BValue);
int CompareUInt64(const void * AValue, const void * BValue);

bool GreaterInt8(const void * AValue, const void * BValue);
bool GreaterInt16(const void * AValue, const void * BValue);
bool GreaterInt32(const void * AValue, const void * BValue);
bool GreaterInt64(const void * AValue, const void * BValue);

bool GreaterUInt8(const void * AValue, const void * BValue);
bool GreaterUInt16(const void * AValue, const void * BValue);
bool GreaterUInt32(const void * AValue, const void * BValue);
bool GreaterUInt64(const void * AValue, const void * BValue);

bool LessInt8(const void * AValue, const void * BValue);
bool LessInt16(const void * AValue, const void * BValue);
bool LessInt32(const void * AValue, const void * BValue);
bool LessInt64(const void * AValue, const void * BValue);

bool LessUInt8(const void * AValue, const void * BValue);
bool LessUInt16(const void * AValue, const void * BValue);
bool LessUInt32(const void * AValue, const void * BValue);
bool LessUInt64(const void * AValue, const void * BValue);

NCLResultType PrintInt8(const void * Value);
NCLResultType PrintInt16(const void * Value);
NCLResultType PrintInt32(const void * Value);
NCLResultType PrintInt64(const void * Value);

NCLResultType PrintUInt8(const void * Value);
NCLResultType PrintUInt16(const void * Value);
NCLResultType PrintUInt32(const void * Value);
NCLResultType PrintUInt64(const void * Value);

/**
 * Swap
 *
 * Notes:
 * - Pointers must not be NULL.
 * - Regions of Size bytes must be identical or not overlap. Partial overlap gives an unpredictable result.
 */
void Swap(void * AValue, void * BValue, size_t Size);

/**
 * StrDup
 *
 * Notes:
 * - Returns NULL in case of not enough memory
 * - Delete string with free()
 */
char * StrDup(const char * Str);

#endif // UTILS_H
