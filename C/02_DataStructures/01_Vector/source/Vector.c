/**
 * Vector.c
 */

#include "Vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Helper Functions
 */

static size_t Vector_NextCapacity(const VectorType * Vector)
{
  size_t MaxCapacity = 0U;
  size_t NextCapacity = 0U;

  MaxCapacity = SIZE_MAX / Vector->ElementSize;

  if (Vector->Capacity == 0U)
  {
    // If the MaxCapacity value turns out to be very small
    // due to the enormous size of an element,
    // then a capacity of 4U might not be available.
    // In such a case, the MaxCapacity value should be returned.
    NextCapacity = (MaxCapacity < 4U) ? MaxCapacity : 4U;
  }
  else if (Vector->Capacity <= MaxCapacity / 2U)
  {
    NextCapacity = Vector->Capacity * 2U;
  }
  else
  {
    NextCapacity = MaxCapacity;
  }

  return NextCapacity;
}

static const void * Vector_AddrAtConst(const VectorType * Vector, size_t Index)
{
  const void * Addr = NULL;

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (Vector->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Index >= Vector->Capacity)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Addr = Vector->Data + Vector->ElementSize * Index;
  }

  return Addr;
}

static void * Vector_AddrAt(VectorType * Vector, size_t Index)
{
  void * Addr = NULL;

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (Vector->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Index >= Vector->Capacity)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Addr = Vector->Data + Vector->ElementSize * Index;
  }

  return Addr;
}

/**
 * Lifecycle
 */

NCLResultType Vector_Init(VectorType * Vector, size_t ElementSize)
{
  NCLResultType Result = NCL_E_OK;

  if (Vector == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    Vector->Data = NULL;
    Vector->ElementSize = ElementSize;
    Vector->Capacity = 0U;
    Vector->Size = 0U;
  }

  return Result;
}

NCLResultType Vector_Deinit(VectorType * Vector)
{
  NCLResultType Result = NCL_E_OK;

  if (Vector == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    if (Vector->Data != NULL)
    {
      free(Vector->Data);
    }

    Vector->Data = NULL;
    Vector->ElementSize = 0U;
    Vector->Capacity = 0U;
    Vector->Size = 0U;
  }

  return Result;
}

/**
 * State Operations
 */

NCLResultType Vector_Reserve(VectorType * Vector, size_t Capacity)
{
  NCLResultType Result = NCL_E_OK;
  void * p = NULL;

  if (Vector == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (Vector->ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (Capacity <= Vector->Capacity)
  {
    // Do nothing.
  }
  else if (Capacity > SIZE_MAX / Vector->ElementSize)
  {
    Result = NCL_E_ALLOC_FAILED;
  }
  else
  {
    p = realloc(Vector->Data, Vector->ElementSize * Capacity );

    if (p == NULL)
    {
      Result = NCL_E_ALLOC_FAILED;
    }
    else
    {
      memset((uint8_t *)p + Vector->ElementSize * Vector->Capacity, 0U, Vector->ElementSize * (Capacity - Vector->Capacity));

      Vector->Data = p;
      Vector->Capacity = Capacity;
    }
  }

  return Result;
}

NCLResultType Vector_Resize(VectorType * Vector, size_t Size)
{
  NCLResultType Result = NCL_E_OK;

  if (Vector == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (Vector->ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    if (Size > Vector->Capacity)
    {
      size_t NextCapacity = Vector_NextCapacity(Vector);

      Result = Vector_Reserve(Vector, (Size > NextCapacity) ? Size : NextCapacity);
    }

    if (Result == NCL_E_OK)
    {
      if (Size > Vector->Size)
      {
        memset(Vector_AddrAt(Vector, Vector->Size), 0, Vector->ElementSize * (Size - Vector->Size));
      }

      Vector->Size = Size;
    }
  }

  return Result;
}

NCLResultType Vector_Clear(VectorType * Vector)
{
  NCLResultType Result = NCL_E_OK;

  if (Vector == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = Vector_Resize(Vector, 0U);
  }

  return Result;
}

size_t Vector_Size(const VectorType * Vector)
{
  size_t Result = 0U;

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = Vector->Size;
  }

  return Result;
}

bool Vector_IsEmpty(const VectorType * Vector)
{
  bool Result = true;

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = (Vector->Size == 0U);
  }

  return Result;
}

/**
 * Iterators
 */

NCLResultType VectorIterator_InitConst(ConstVectorIteratorType * It)
{
  NCLResultType Result = NCL_E_OK;

  if (It == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    It->Vector = NULL;
    It->Index = 0U;
  }

  return Result;
}

NCLResultType VectorIterator_Init(VectorIteratorType * It)
{
  NCLResultType Result = NCL_E_OK;

  if (It == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    It->Vector = NULL;
    It->Index = 0U;
  }

  return Result;
}

ConstVectorIteratorType VectorIterator_NextConst(ConstVectorIteratorType It)
{
  ConstVectorIteratorType Next;

  VectorIterator_InitConst(&Next);

  if (It.Vector == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Index > It.Vector->Size)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Index == It.Vector->Size)
  {
    // Cannot move past end.
    // Do nothing.
  }
  else
  {
    Next.Vector = It.Vector;
    Next.Index = It.Index + 1;
  }

  return Next;
}

VectorIteratorType VectorIterator_Next(VectorIteratorType It)
{
  VectorIteratorType Next;

  VectorIterator_Init(&Next);

  if (It.Vector == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Index > It.Vector->Size)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Index == It.Vector->Size)
  {
    // Cannot move past end.
    // Do nothing.
  }
  else
  {
    Next.Vector = It.Vector;
    Next.Index = It.Index + 1;
  }

  return Next;
}

ConstVectorIteratorType VectorIterator_PrevConst(ConstVectorIteratorType It)
{
  ConstVectorIteratorType Prev;

  VectorIterator_InitConst(&Prev);

  if (It.Vector == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Index > It.Vector->Size)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Index == 0U)
  {
    // Cannot move before begin;
    // Do nothing.
  }
  else
  {
    Prev.Vector = It.Vector;
    Prev.Index = It.Index - 1U;
  }

  return Prev;
}

VectorIteratorType VectorIterator_Prev(VectorIteratorType It)
{
  VectorIteratorType Prev;

  VectorIterator_Init(&Prev);

  if (It.Vector == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Index > It.Vector->Size)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Index == 0U)
  {
    // Cannot move before begin;
    // Do nothing.
  }
  else
  {
    Prev.Vector = It.Vector;
    Prev.Index = It.Index - 1U;
  }

  return Prev;
}

const void * VectorIterator_DataConst(ConstVectorIteratorType It)
{
  const void * Data = NULL;

  if (It.Vector == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Vector->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Vector->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Index >= It.Vector->Size)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Data = Vector_AddrAtConst(It.Vector, It.Index);
  }

  return Data;
}

void * VectorIterator_Data(VectorIteratorType It)
{
  void * Data = NULL;

  if (It.Vector == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Vector->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Vector->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Index >= It.Vector->Size)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Data = Vector_AddrAt(It.Vector, It.Index);
  }

  return Data;
}

bool VectorIterator_EqualConst(ConstVectorIteratorType It1, ConstVectorIteratorType It2)
{
  bool Result = false;

  if (It1.Vector == NULL || It2.Vector == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It1.Index > It1.Vector->Size || It2.Index > It2.Vector->Size)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Result = ((It1.Vector == It2.Vector) && (It1.Index == It2.Index));
  }

  return Result;
}

bool VectorIterator_Equal(VectorIteratorType It1, VectorIteratorType It2)
{
  bool Result = false;

  if (It1.Vector == NULL || It2.Vector == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It1.Index > It1.Vector->Size || It2.Index > It2.Vector->Size)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Result = ((It1.Vector == It2.Vector) && (It1.Index == It2.Index));
  }

  return Result;
}

ConstVectorIteratorType Vector_BeginConst(const VectorType * Vector)
{
  ConstVectorIteratorType Begin;

  VectorIterator_InitConst(&Begin);

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Begin.Vector = Vector;
    Begin.Index = 0U;
  }

  return Begin;
}

VectorIteratorType Vector_Begin(VectorType * Vector)
{
  VectorIteratorType Begin;

  VectorIterator_Init(&Begin);

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Begin.Vector = Vector;
    Begin.Index = 0U;
  }

  return Begin;
}

ConstVectorIteratorType Vector_EndConst(const VectorType * Vector)
{
  ConstVectorIteratorType End;

  VectorIterator_InitConst(&End);

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    End.Vector = Vector;
    End.Index = Vector->Size;
  }

  return End;
}

VectorIteratorType Vector_End(VectorType * Vector)
{
  VectorIteratorType End;

  VectorIterator_Init(&End);

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    End.Vector = Vector;
    End.Index = Vector->Size;
  }

  return End;
}

/**
 * Access
 */

const void * Vector_DataAtConst(const VectorType * Vector, size_t Index)
{
  const void * Data = NULL;

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (Vector->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Index >= Vector->Size)
  {
    // NCL_E_OUT_OF_RANGE
    // Do nothing.
  }
  else
  {
    Data = Vector_AddrAtConst(Vector, Index);
  }

  return Data;
}

void * Vector_DataAt(VectorType * Vector, size_t Index)
{
  void * Data = NULL;

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (Vector->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Index >= Vector->Size)
  {
    // NCL_E_OUT_OF_RANGE
    // Do nothing.
  }
  else
  {
    Data = Vector_AddrAt(Vector, Index);
  }

  return Data;
}

NCLResultType Vector_Get(const VectorType * Vector, size_t Index, void * Value)
{
  NCLResultType Result = NCL_E_OK;
  const void * Addr = NULL;

  if (Vector == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (Vector->ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (Vector->Data == NULL)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (Index >= Vector->Size)
  {
    Result = NCL_E_OUT_OF_RANGE;
  }
  else
  {
    Addr = Vector_AddrAtConst(Vector, Index);

    if (Addr == NULL)
    {
      Result = NCL_E_OUT_OF_RANGE;
    }
    else
    {
      memcpy(Value, Addr, Vector->ElementSize);
    }
  }

  return Result;
}

NCLResultType Vector_Set(VectorType * Vector, size_t Index, const void * Value)
{
  NCLResultType Result = NCL_E_OK;
  void * Addr = NULL;

  if (Vector == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (Vector->ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (Vector->Data == NULL)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (Index >= Vector->Size)
  {
    Result = NCL_E_OUT_OF_RANGE;
  }
  else
  {
    Addr = Vector_AddrAt(Vector, Index);

    if (Addr == NULL)
    {
      Result = NCL_E_OUT_OF_RANGE;
    }
    else
    {
      memcpy(Addr, Value, Vector->ElementSize);
    }
  }

  return Result;
}

/**
 * Modification
 */

VectorIteratorType Vector_Insert(VectorType * Vector, VectorIteratorType It, const void * Value)
{
  VectorIteratorType Result;
  NCLResultType Error = NCL_E_OK;

  VectorIterator_Init(&Result);

  if (Vector == NULL || Value == NULL)
  {
    Error = NCL_E_NULL_PTR;
  }
  else if (Vector->ElementSize == 0U)
  {
    Error = NCL_E_INVALID_ARGUMENT;
  }
  else if (It.Vector != Vector)
  {
    Error = NCL_E_INVALID_ARGUMENT;
  }
  else if (It.Index > Vector->Size)
  {
    Error = NCL_E_INVALID_ARGUMENT;
  }
  else if (Vector->Size == SIZE_MAX)
  {
    Error = NCL_E_ALLOC_FAILED;
  }
  else
  {
    if (Vector->Size == Vector->Capacity)
    {
      size_t NextCapacity = Vector_NextCapacity(Vector);

      Error = Vector_Reserve(Vector, (Vector->Size + 1 > NextCapacity) ? Vector->Size + 1 : NextCapacity);
    }

    if (Error == NCL_E_OK)
    {
      if (It.Index == Vector->Size)
      {
        // Insertion to the end.
        // Nothing to move.
        // Do nothing.
      }
      else
      {
        memmove(Vector_AddrAt(Vector, It.Index + 1U), Vector_AddrAt(Vector, It.Index), Vector->ElementSize * (Vector->Size - It.Index));
      }

      memcpy(Vector_AddrAt(Vector, It.Index), Value, Vector->ElementSize);

      Vector->Size++;

      Result = It;
    }
  }

  return Result;
}

VectorIteratorType Vector_Erase(VectorType * Vector, VectorIteratorType It)
{
  VectorIteratorType Result;

  VectorIterator_Init(&Result);

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (Vector->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Vector != Vector)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Index >= Vector->Size)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    if (It.Index == Vector->Size - 1U)
    {
      // Erase of the last.
      // Nothing to move.
      // Do nothing.
    }
    else
    {
      memmove(Vector_AddrAt(Vector, It.Index), Vector_AddrAt(Vector, It.Index + 1U), Vector->ElementSize * (Vector->Size - (It.Index + 1U)));
    }

    Vector->Size--;

    Result = It;
  }

  return Result;
}

/**
 * Edge Operations
 */

const void * Vector_FrontConst(const VectorType * Vector)
{
  const void * Front = NULL;

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (Vector->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector_IsEmpty(Vector))
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Front = Vector_AddrAtConst(Vector, 0U);
  }

  return Front;
}

void * Vector_Front(VectorType * Vector)
{
  void * Front = NULL;

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (Vector->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector_IsEmpty(Vector))
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Front = Vector_AddrAt(Vector, 0U);
  }

  return Front;
}

const void * Vector_BackConst(const VectorType * Vector)
{
  const void * Back = NULL;

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (Vector->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector_IsEmpty(Vector))
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Back = Vector_AddrAtConst(Vector, Vector->Size - 1U);
  }

  return Back;
}

void * Vector_Back(VectorType * Vector)
{
  void * Back = NULL;

  if (Vector == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (Vector->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector->Data == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Vector_IsEmpty(Vector))
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Back = Vector_AddrAt(Vector, Vector->Size - 1U);
  }

  return Back;
}

NCLResultType Vector_PushFront(VectorType * Vector, const void * Value)
{
  NCLResultType Result = NCL_E_OK;
  VectorIteratorType It;

  VectorIterator_Init(&It);

  if (Vector == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    It = Vector_Insert(Vector, Vector_Begin(Vector), Value);
    Result = (It.Vector == Vector) ? NCL_E_OK : NCL_E_NOT_OK;
  }

  return Result;
}

NCLResultType Vector_PushBack(VectorType * Vector, const void * Value)
{
  NCLResultType Result = NCL_E_OK;
  VectorIteratorType It;

  VectorIterator_Init(&It);

  if (Vector == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    It = Vector_Insert(Vector, Vector_End(Vector), Value);
    Result = (It.Vector == Vector) ? NCL_E_OK : NCL_E_NOT_OK;
  }

  return Result;
}

NCLResultType Vector_PopFront(VectorType * Vector)
{
  NCLResultType Result = NCL_E_OK;
  VectorIteratorType It;

  VectorIterator_Init(&It);

  if (Vector == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (Vector_IsEmpty(Vector))
  {
    Result = NCL_E_EMPTY;
  }
  else
  {
    It = Vector_Erase(Vector, Vector_Begin(Vector));
    Result = (It.Vector == Vector) ? NCL_E_OK : NCL_E_NOT_OK;
  }

  return Result;
}

NCLResultType Vector_PopBack(VectorType * Vector)
{
  NCLResultType Result = NCL_E_OK;
  VectorIteratorType It;

  VectorIterator_Init(&It);

  if (Vector == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (Vector_IsEmpty(Vector))
  {
    Result = NCL_E_EMPTY;
  }
  else
  {
    It = Vector_Erase(Vector, VectorIterator_Prev(Vector_End(Vector)));
    Result = (It.Vector == Vector) ? NCL_E_OK : NCL_E_NOT_OK;
  }

  return Result;
}

/**
 * Debug
 */

NCLResultType Vector_Print(const VectorType * Vector, PrintDataFunctionType PrintData)
{
  NCLResultType Result = NCL_E_OK;

  if (Vector == NULL || PrintData == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (Vector->ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    printf("Vector Data: \n");

    for (size_t i = 0; i < Vector->Capacity; i++)
    {
      PrintData(&Vector->Data[Vector->ElementSize * i]);
    }

    printf("\n");

    printf("Vector->ElementSize     : %zu\n", Vector->ElementSize);
    printf("Vector->Capacity        : %zu\n", Vector->Capacity);
    printf("Vector->Size            : %zu\n", Vector->Size);

    printf("\n");
  }

  return Result;
}
