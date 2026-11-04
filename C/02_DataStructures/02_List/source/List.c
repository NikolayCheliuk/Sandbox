/**
 * List.c
 */

#include "List.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Helper Functions
 */

static ListDataNodeType * List_CreateDataNode(const ListType * List)
{
  ListDataNodeType * Node = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Node = malloc(sizeof(ListDataNodeType) + List->ElementSize);

    if (Node == NULL)
    {
      // NCL_E_ALLOC_FAILED
      // Do nothing.
    }
    else
    {
      Node->Base.Prev = NULL;
      Node->Base.Next = NULL;
    }
  }

  return Node;
}

static void List_DeleteDataNode(ListDataNodeType * Node)
{
  if (Node == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    free(Node);
  }
}

static const ListNodeType * List_HeadConst(const ListType * List)
{
  const ListNodeType * Node = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (List->Sentinel.Next == &List->Sentinel)
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Node = List->Sentinel.Next;
  }

  return Node;
}

static ListNodeType * List_Head(ListType * List)
{
  ListNodeType * Node = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (List->Sentinel.Next == &List->Sentinel)
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Node = List->Sentinel.Next;
  }

  return Node;
}

static const ListNodeType * List_TailConst(const ListType * List)
{
  const ListNodeType * Node = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (List->Sentinel.Prev == &List->Sentinel)
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Node = List->Sentinel.Prev;
  }

  return Node;
}

static ListNodeType * List_Tail(ListType * List)
{
  ListNodeType * Node = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (List->Sentinel.Prev == &List->Sentinel)
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Node = List->Sentinel.Prev;
  }

  return Node;
}

static const ListNodeType * List_NodeAtConst(const ListType * List, size_t Index)
{
  const ListNodeType * Node = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (Index >= List->Size)
  {
    // NCL_E_OUT_OF_RANGE
    // Do nothing.
  }
  else
  {
    if (Index < List->Size / 2U)
    {
      Node = List->Sentinel.Next;

      for (size_t i = 0U; i < Index; i++)
      {
        Node = Node->Next;
      }
    }
    else
    {
      Node = List->Sentinel.Prev;

      for (size_t i = List->Size - 1U; i > Index; i--)
      {
        Node =  Node->Prev;
      }
    }
  }

  return Node;
}

static ListNodeType * List_NodeAt(ListType * List, size_t Index)
{
  ListNodeType * Node = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (Index >= List->Size)
  {
    // NCL_E_OUT_OF_RANGE
    // Do nothing.
  }
  else
  {
    if (Index < List->Size / 2U)
    {
      Node = List->Sentinel.Next;

      for (size_t i = 0U; i < Index; i++)
      {
        Node = Node->Next;
      }
    }
    else
    {
      Node = List->Sentinel.Prev;

      for (size_t i = List->Size - 1U; i > Index; i--)
      {
        Node =  Node->Prev;
      }
    }
  }

  return Node;
}

/**
 * Lifecycle
 */

NCLResultType List_Init(ListType * List, size_t ElementSize)
{
  NCLResultType Result = NCL_E_OK;

  if (List == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (ElementSize == 0U || ElementSize > SIZE_MAX - sizeof(ListDataNodeType))
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    List->Sentinel.Next = &List->Sentinel;
    List->Sentinel.Prev = &List->Sentinel;
    List->ElementSize = ElementSize;
    List->Size = 0U;
  }

  return Result;
}

NCLResultType List_Deinit(ListType * List)
{
  NCLResultType Result = NCL_E_OK;

  if (List == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = List_Clear(List);

    if (Result == NCL_E_OK)
    {
      List->ElementSize = 0U;
    }
  }

  return Result;
}

/**
 * State Operations
 */

NCLResultType List_Clear(ListType * List)
{
  NCLResultType Result = NCL_E_OK;
  ListNodeType * Node = NULL;
  ListNodeType * Next = NULL;

  if (List == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Node = List->Sentinel.Next;

    while (Node != &List->Sentinel)
    {
      Next = Node->Next;
      List_DeleteDataNode((ListDataNodeType *)Node);
      Node = Next;
    }

    List->Sentinel.Next = &List->Sentinel;
    List->Sentinel.Prev = &List->Sentinel;
    List->Size = 0U;
  }

  return Result;
}

size_t List_Size(const ListType * List)
{
  size_t Result = 0U;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = List->Size;
  }

  return Result;
}

bool List_IsEmpty(const ListType * List)
{
  bool Result = true;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = (List->Size == 0U);
  }

  return Result;
}

/**
 * Iterators
 */

NCLResultType ListIterator_InitConst(ConstListIteratorType * It)
{
  NCLResultType Result = NCL_E_OK;

  if (It == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    It->List = NULL;
    It->Node = NULL;
  }

  return Result;
}

NCLResultType ListIterator_Init(ListIteratorType * It)
{
  NCLResultType Result = NCL_E_OK;

  if (It == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    It->List = NULL;
    It->Node = NULL;
  }

  return Result;
}

ConstListIteratorType ListIterator_NextConst(ConstListIteratorType It)
{
  ConstListIteratorType Next;

  ListIterator_InitConst(&Next);

  if (It.List == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == &It.List->Sentinel)
  {
    // Cannot move past end.
    // Do nothing.
  }
  else
  {
    Next.List = It.List;
    Next.Node = It.Node->Next;
  }

  return Next;
}

ListIteratorType ListIterator_Next(ListIteratorType It)
{
  ListIteratorType Next;

  ListIterator_Init(&Next);

  if (It.List == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == &It.List->Sentinel)
  {
    // Cannot move past end.
    // Do nothing.
  }
  else
  {
    Next.List = It.List;
    Next.Node = It.Node->Next;
  }

  return Next;
}

ConstListIteratorType ListIterator_PrevConst(ConstListIteratorType It)
{
  ConstListIteratorType Prev;

  ListIterator_InitConst(&Prev);

  if (It.List == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == It.List->Sentinel.Next)
  {
    // Cannot move before begin.
    // Do nothing.
  }
  else
  {
    Prev.List = It.List;
    Prev.Node = It.Node->Prev;
  }

  return Prev;
}

ListIteratorType ListIterator_Prev(ListIteratorType It)
{
  ListIteratorType Prev;

  ListIterator_Init(&Prev);

  if (It.List == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == It.List->Sentinel.Next)
  {
    // Cannot move before begin.
    // Do nothing.
  }
  else
  {
    Prev.List = It.List;
    Prev.Node = It.Node->Prev;
  }

  return Prev;
}

const void * ListIterator_DataConst(ConstListIteratorType It)
{
  const void * Data = NULL;

  if (It.List == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.List->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == &It.List->Sentinel)
  {
    // Data cannot be acquired from End.
    // Do nothing.
  }
  else
  {
    Data = ((const ListDataNodeType *)It.Node)->Data;
  }

  return Data;
}

void * ListIterator_Data(ListIteratorType It)
{
  void * Data = NULL;

  if (It.List == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.List->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == &It.List->Sentinel)
  {
    // Data cannot be acquired from End.
    // Do nothing.
  }
  else
  {
    Data = ((ListDataNodeType *)It.Node)->Data;
  }

  return Data;
}

bool ListIterator_EqualConst(ConstListIteratorType It1, ConstListIteratorType It2)
{
  bool Result = false;

  if (It1.List == NULL || It2.List == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It1.Node == NULL || It2.Node == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Result = ((It1.List == It2.List) && (It1.Node == It2.Node));
  }

  return Result;
}

bool ListIterator_Equal(ListIteratorType It1, ListIteratorType It2)
{
  bool Result = false;

  if (It1.List == NULL || It2.List == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It1.Node == NULL || It2.Node == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Result = ((It1.List == It2.List) && (It1.Node == It2.Node));
  }

  return Result;
}

ConstListIteratorType List_BeginConst(const ListType * List)
{
  ConstListIteratorType Begin;

  ListIterator_InitConst(&Begin);

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Begin.List = List;
    Begin.Node = List->Sentinel.Next;
  }

  return Begin;
}

ListIteratorType List_Begin(ListType * List)
{
  ListIteratorType Begin;

  ListIterator_Init(&Begin);

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Begin.List = List;
    Begin.Node = List->Sentinel.Next;
  }

  return Begin;
}

ConstListIteratorType List_EndConst(const ListType * List)
{
  ConstListIteratorType End;

  ListIterator_InitConst(&End);

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    End.List = List;
    End.Node = &List->Sentinel;
  }

  return End;
}

ListIteratorType List_End(ListType * List)
{
  ListIteratorType End;

  ListIterator_Init(&End);

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    End.List = List;
    End.Node = &List->Sentinel;
  }

  return End;
}

/**
 * Access
 */

const void * List_DataAtConst(const ListType * List, size_t Index)
{
  const void * Data = NULL;
  const ListNodeType * Node = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (List->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Index >= List->Size)
  {
    // NCL_E_OUT_OF_RANGE
    // Do nothing.
  }
  else
  {
    Node = List_NodeAtConst(List, Index);

    if (Node == NULL)
    {
      // NCL_E_OUT_OF_RANGE
      // Do nothing.
    }
    else
    {
      Data = ((const ListDataNodeType *)Node)->Data;
    }
  }

  return Data;
}

void * List_DataAt(ListType * List, size_t Index)
{
  void * Data = NULL;
  ListNodeType * Node = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (List->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (Index >= List->Size)
  {
    // NCL_E_OUT_OF_RANGE
    // Do nothing.
  }
  else
  {
    Node = List_NodeAt(List, Index);

    if (Node == NULL)
    {
      // NCL_E_OUT_OF_RANGE
      // Do nothing.
    }
    else
    {
      Data = ((ListDataNodeType *)Node)->Data;
    }
  }

  return Data;
}

NCLResultType List_Get(const ListType * List, size_t Index, void * Value)
{
  NCLResultType Result = NCL_E_OK;
  const ListNodeType * Node = NULL;

  if (List == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (List->ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (Index >= List->Size)
  {
    Result = NCL_E_OUT_OF_RANGE;
  }
  else
  {
    Node = List_NodeAtConst(List, Index);

    if (Node == NULL)
    {
      Result = NCL_E_OUT_OF_RANGE;
    }
    else
    {
      memcpy(Value, ((const ListDataNodeType *)Node)->Data, List->ElementSize);
    }
  }

  return Result;
}

NCLResultType List_Set(ListType * List, size_t Index, const void * Value)
{
  NCLResultType Result = NCL_E_OK;
  ListNodeType * Node = NULL;

  if (List == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (List->ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (Index >= List->Size)
  {
    Result = NCL_E_OUT_OF_RANGE;
  }
  else
  {
    Node = List_NodeAt(List, Index);

    if (Node == NULL)
    {
      Result = NCL_E_OUT_OF_RANGE;
    }
    else
    {
      memcpy(((ListDataNodeType *)Node)->Data, Value, List->ElementSize);
    }
  }

  return Result;
}

/**
 * Modification
 */

ListIteratorType List_Insert(ListType * List, ListIteratorType It, const void * Value)
{
  ListIteratorType Result;
  ListDataNodeType * NewNode = NULL;

  ListIterator_Init(&Result);

  if (List == NULL || Value == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (List->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.List != List)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (List->Size == SIZE_MAX)
  {
    // NCL_E_ALLOC_FAILED
    // Do nothing.
  }
  else
  {
    NewNode = List_CreateDataNode(List);

    if (NewNode == NULL)
    {
      // NCL_E_ALLOC_FAILED
      // Do nothing.
    }
    else
    {
      memcpy(NewNode->Data, Value, List->ElementSize);

      Result.List = List;
      Result.Node = &NewNode->Base;

      NewNode->Base.Next = It.Node;
      NewNode->Base.Prev = It.Node->Prev;

      It.Node->Prev->Next = &NewNode->Base;
      It.Node->Prev = &NewNode->Base;

      List->Size++;
    }
  }

  return Result;
}

ListIteratorType List_Erase(ListType * List, ListIteratorType It)
{
  ListIteratorType Result;

  ListIterator_Init(&Result);

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (List->ElementSize == 0U)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.List != List)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == &List->Sentinel)
  {
    // End cannot be removed;
    // Do nothing.
  }
  else
  {
    Result.List = List;
    Result.Node = It.Node->Next;

    It.Node->Prev->Next = It.Node->Next;
    It.Node->Next->Prev = It.Node->Prev;

    List_DeleteDataNode((ListDataNodeType *)It.Node);

    List->Size--;
  }

  return Result;
}

/**
 * Edge Operations
 */

const void * List_FrontConst(const ListType * List)
{
  const void * Front = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (List_IsEmpty(List))
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Front = ((const ListDataNodeType *)List->Sentinel.Next)->Data;
  }

  return Front;
}

void * List_Front(ListType * List)
{
  void * Front = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (List_IsEmpty(List))
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Front = ((ListDataNodeType *)List->Sentinel.Next)->Data;
  }

  return Front;
}

const void * List_BackConst(const ListType * List)
{
  const void * Back = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (List_IsEmpty(List))
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Back = ((const ListDataNodeType *)List->Sentinel.Prev)->Data;
  }

  return Back;
}

void * List_Back(ListType * List)
{
  void * Back = NULL;

  if (List == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (List_IsEmpty(List))
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Back = ((ListDataNodeType *)List->Sentinel.Prev)->Data;
  }

  return Back;
}

NCLResultType List_PushFront(ListType * List, const void * Value)
{
  NCLResultType Result = NCL_E_OK;
  ListIteratorType It;

  ListIterator_Init(&It);

  if (List == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    It = List_Insert(List, List_Begin(List), Value);
    Result = (It.List == List) ? NCL_E_OK : NCL_E_NOT_OK;
  }

  return Result;
}

NCLResultType List_PushBack(ListType * List, const void * Value)
{
  NCLResultType Result = NCL_E_OK;
  ListIteratorType It;

  ListIterator_Init(&It);

  if (List == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    It = List_Insert(List, List_End(List), Value);
    Result = (It.List == List) ? NCL_E_OK : NCL_E_NOT_OK;
  }

  return Result;
}

NCLResultType List_PopFront(ListType * List)
{
  NCLResultType Result = NCL_E_OK;
  ListIteratorType It;

  ListIterator_Init(&It);

  if (List == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (List_IsEmpty(List))
  {
    Result = NCL_E_EMPTY;
  }
  else
  {
    It = List_Erase(List, List_Begin(List));
    Result = (It.List == List) ? NCL_E_OK : NCL_E_NOT_OK;
  }

  return Result;
}

NCLResultType List_PopBack(ListType * List)
{
  NCLResultType Result = NCL_E_OK;
  ListIteratorType It;

  ListIterator_Init(&It);

  if (List == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (List_IsEmpty(List))
  {
    Result = NCL_E_EMPTY;
  }
  else
  {
    It = List_Erase(List, ListIterator_Prev(List_End(List)));
    Result = (It.List == List) ? NCL_E_OK : NCL_E_NOT_OK;
  }

  return Result;
}

/**
 * Debug
 */

NCLResultType List_Print(const ListType * List, PrintDataFunctionType PrintData)
{
  NCLResultType Result = NCL_E_OK;
  const ListNodeType * Node = NULL;

  if (List == NULL || PrintData == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (List->ElementSize == 0U)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    printf("List Data: \n");

    for (Node = List->Sentinel.Next; Node != &List->Sentinel; Node = Node->Next)
    {
      PrintData(((const ListDataNodeType *)Node)->Data);
    }

    printf("\n");

    printf("List->ElementSize       : %zu\n", List->ElementSize);
    printf("List->Size              : %zu\n", List->Size);

    printf("\n");
  }

  return Result;
}
