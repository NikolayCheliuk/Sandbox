/**
 * ErrorCodes.h
 *
 * Notes:
 *
 */

#ifndef ERRORCODES_H
#define ERRORCODES_H

typedef enum
{
  NCL_E_OK = 0U,
  NCL_E_NOT_OK,

  NCL_E_INVALID_ARGUMENT,
  NCL_E_NULL_PTR,
  NCL_E_OUT_OF_RANGE,

  NCL_E_EMPTY,
  NCL_E_FULL,

  NCL_E_NOT_FOUND,
  NCL_E_ALREADY_EXISTS,

  NCL_E_ALLOC_FAILED
} NCLResultType;

#endif // ERRORCODES_H
