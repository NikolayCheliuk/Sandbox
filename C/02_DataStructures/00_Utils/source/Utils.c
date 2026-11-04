/**
 * Utils.c
 */

#include "Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Print Functions
 */

NCLResultType PrintInt8(const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    printf("[%" PRId8 "]", *(const int8_t *)Value);
  }

  return Result;
}

NCLResultType PrintInt16(const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    printf("[%" PRId16 "]", *(const int16_t *)Value);
  }

  return Result;
}

NCLResultType PrintInt32(const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    printf("[%" PRId32 "]", *(const int32_t *)Value);
  }

  return Result;
}

NCLResultType PrintInt64(const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    printf("[%" PRId64 "]", *(const int64_t *)Value);
  }

  return Result;
}

NCLResultType PrintUInt8(const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    printf("[%" PRIu8 "]", *(const uint8_t *)Value);
  }

  return Result;
}

NCLResultType PrintUInt16(const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    printf("[%" PRIu16 "]", *(const uint16_t *)Value);
  }

  return Result;
}

NCLResultType PrintUInt32(const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    printf("[%" PRIu32 "]", *(const uint32_t *)Value);
  }

  return Result;
}

NCLResultType PrintUInt64(const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  if (Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    printf("[%" PRIu64 "]", *(const uint64_t *)Value);
  }

  return Result;
}

/**
 * Compare Functions
 */

int CompareInt8(const void * AValue, const void * BValue)
{
  int Result = 0;

  if (GreaterInt8(AValue, BValue))
  {
    Result = 1;
  }
  else if (LessInt8(AValue, BValue))
  {
    Result = -1;
  }

  return Result;
}

int CompareInt16(const void * AValue, const void * BValue)
{
  int Result = 0;

  if (GreaterInt16(AValue, BValue))
  {
    Result = 1;
  }
  else if (LessInt16(AValue, BValue))
  {
    Result = -1;
  }

  return Result;
}

int CompareInt32(const void * AValue, const void * BValue)
{
  int Result = 0;

  if (GreaterInt32(AValue, BValue))
  {
    Result = 1;
  }
  else if (LessInt32(AValue, BValue))
  {
    Result = -1;
  }

  return Result;
}

int CompareInt64(const void * AValue, const void * BValue)
{
  int Result = 0;

  if (GreaterInt64(AValue, BValue))
  {
    Result = 1;
  }
  else if (LessInt64(AValue, BValue))
  {
    Result = -1;
  }

  return Result;
}

int CompareUInt8(const void * AValue, const void * BValue)
{
  int Result = 0;

  if (GreaterUInt8(AValue, BValue))
  {
    Result = 1;
  }
  else if (LessUInt8(AValue, BValue))
  {
    Result = -1;
  }

  return Result;
}

int CompareUInt16(const void * AValue, const void * BValue)
{
  int Result = 0;

  if (GreaterUInt16(AValue, BValue))
  {
    Result = 1;
  }
  else if (LessUInt16(AValue, BValue))
  {
    Result = -1;
  }

  return Result;
}

int CompareUInt32(const void * AValue, const void * BValue)
{
  int Result = 0;

  if (GreaterUInt32(AValue, BValue))
  {
    Result = 1;
  }
  else if (LessUInt32(AValue, BValue))
  {
    Result = -1;
  }

  return Result;
}

int CompareUInt64(const void * AValue, const void * BValue)
{
  int Result = 0;

  if (GreaterUInt64(AValue, BValue))
  {
    Result = 1;
  }
  else if (LessUInt64(AValue, BValue))
  {
    Result = -1;
  }

  return Result;
}

/**
 * Greater Functions
 */

bool GreaterInt8(const void * AValue, const void * BValue)
{
  return (*(const int8_t *)AValue > *(const int8_t *)BValue);
}

bool GreaterInt16(const void * AValue, const void * BValue)
{
  return (*(const int16_t *)AValue > *(const int16_t *)BValue);
}

bool GreaterInt32(const void * AValue, const void * BValue)
{
  return (*(const int32_t *)AValue > *(const int32_t *)BValue);
}

bool GreaterInt64(const void * AValue, const void * BValue)
{
  return (*(const int64_t *)AValue > *(const int64_t *)BValue);
}

bool GreaterUInt8(const void * AValue, const void * BValue)
{
  return (*(const uint8_t *)AValue > *(const uint8_t *)BValue);
}

bool GreaterUInt16(const void * AValue, const void * BValue)
{
  return (*(const uint16_t *)AValue > *(const uint16_t *)BValue);
}

bool GreaterUInt32(const void * AValue, const void * BValue)
{
  return (*(const uint32_t *)AValue > *(const uint32_t *)BValue);
}

bool GreaterUInt64(const void * AValue, const void * BValue)
{
  return (*(const uint64_t *)AValue > *(const uint64_t *)BValue);
}

/**
 * Less Functions
 */

bool LessInt8(const void * AValue, const void * BValue)
{
  return (*(const int8_t *)AValue < *(const int8_t *)BValue);
}

bool LessInt16(const void * AValue, const void * BValue)
{
  return (*(const int16_t *)AValue < *(const int16_t *)BValue);
}

bool LessInt32(const void * AValue, const void * BValue)
{
  return (*(const int32_t *)AValue < *(const int32_t *)BValue);
}

bool LessInt64(const void * AValue, const void * BValue)
{
  return (*(const int64_t *)AValue < *(const int64_t *)BValue);
}

bool LessUInt8(const void * AValue, const void * BValue)
{
  return (*(const uint8_t *)AValue < *(const uint8_t *)BValue);
}

bool LessUInt16(const void * AValue, const void * BValue)
{
  return (*(const uint16_t *)AValue < *(const uint16_t *)BValue);
}

bool LessUInt32(const void * AValue, const void * BValue)
{
  return (*(const uint32_t *)AValue < *(const uint32_t *)BValue);
}

bool LessUInt64(const void * AValue, const void * BValue)
{
  return (*(const uint64_t *)AValue < *(const uint64_t *)BValue);
}

void Swap(void * AValue, void * BValue, size_t Size)
{
  uint8_t Temp;
  uint8_t * A;
  uint8_t * B;

  if (AValue == NULL || BValue == NULL)
  {
    // Do nothing.
  }
  else if (Size == 0U)
  {
    // Do nothing.
  }
  else
  {
    A = AValue;
    B = BValue;

    for (size_t i = 0U; i < Size; i++)
    {
      Temp = A[i];
      A[i] = B[i];
      B[i] = Temp;
    }
  }
}

char * StrDup(const char * Str)
{
  char * Result = NULL;
  size_t Length = 0U;

  if (Str == NULL)
  {
    // Do nothing.
  }
  else
  {
    Length = strlen(Str) + 1U;
    Result = malloc(Length);

    if (Result == NULL)
    {
      // Do nothing.
    }
    else
    {
      memcpy(Result, Str, Length);
    }
  }

  return Result;
}
