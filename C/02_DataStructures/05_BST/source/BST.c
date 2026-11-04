/**
 * BST.c
 */

#include "BST.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Helper Functions
 */

static BSTNodeType * BST_CreateNode(const BSTType * BST)
{
  BSTNodeType * Node = NULL;

  if (BST == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Node = malloc(sizeof(BSTNodeType) + BST->ElementSize);

    if (Node == NULL)
    {
      // NCL_E_ALLOC_FAILED
      // Do nothing.
    }
    else
    {
      Node->Parent = NULL;
      Node->Left = NULL;
      Node->Right = NULL;
    }
  }

  return Node;
}

static void BST_DeleteNode(BSTNodeType * Node)
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

static void BST_EraseNodeRecursive(BSTNodeType * Node)
{
  if (Node == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    BST_EraseNodeRecursive(Node->Left);
    BST_EraseNodeRecursive(Node->Right);
    BST_DeleteNode(Node);
  }
}

static void BST_PrintNode(const BSTNodeType * Node, PrintDataFunctionType PrintData)
{
  if (Node == NULL)
  {
    // Do nothing.
  }
  else
  {
    BST_PrintNode(Node->Left, PrintData);
    PrintData(Node->Data);
    BST_PrintNode(Node->Right, PrintData);
  }
}

/**
 * Lifecycle
 */

NCLResultType BST_Init(BSTType * BST, size_t ElementSize, CompareFunctionType CompareFunction)
{
  NCLResultType Result = NCL_E_OK;

  if (BST == NULL || CompareFunction == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (ElementSize == 0U || ElementSize > SIZE_MAX - sizeof(BSTNodeType))
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    BST->Root = NULL;
    BST->ElementSize = ElementSize;
    BST->Size = 0U;
    BST->CompareFunction = CompareFunction;
  }

  return Result;
}

NCLResultType BST_Deinit(BSTType * BST)
{
  NCLResultType Result = NCL_E_OK;

  if (BST == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    Result = BST_Clear(BST);

    BST->Root = NULL;
    BST->ElementSize = 0U;
    BST->Size = 0U;
    BST->CompareFunction = NULL;
  }

  return Result;
}

/**
 * State Operations
 */

NCLResultType BST_Clear(BSTType * BST)
{
  NCLResultType Result = NCL_E_OK;

  if (BST == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    BST_EraseNodeRecursive(BST->Root);
    BST->Root = NULL;
    BST->Size = 0U;
  }

  return Result;
}

size_t BST_Size(const BSTType * BST)
{
  size_t Result = 0U;

  if (BST == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = BST->Size;
  }

  return Result;
}

bool BST_IsEmpty(const BSTType * BST)
{
  bool Result = true;

  if (BST == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    Result = (BST->Size == 0U);
  }

  return Result;
}

/**
 * Iterators
 */

NCLResultType BSTIterator_InitConst(ConstBSTIteratorType * It)
{
  NCLResultType Result = NCL_E_OK;

  if (It == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    It->BST = NULL;
    It->Node = NULL;
  }

  return Result;
}

NCLResultType BSTIterator_Init(BSTIteratorType * It)
{
  NCLResultType Result = NCL_E_OK;

  if (It == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    It->BST = NULL;
    It->Node = NULL;
  }

  return Result;
}

ConstBSTIteratorType BSTIterator_NextConst(ConstBSTIteratorType It)
{
  if (It.BST == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    BSTIterator_InitConst(&It);
  }
  else if (It.Node == NULL)
  {
    // Cannot move past end.
    BSTIterator_InitConst(&It);
  }
  else
  {
    if (It.Node->Right != NULL)
    {
      It.Node = It.Node->Right;

      while (It.Node->Left != NULL)
      {
        It.Node = It.Node->Left;
      }
    }
    else
    {
      while (It.Node->Parent != NULL && It.Node->Parent->Right == It.Node)
      {
        It.Node = It.Node->Parent;
      }

      It.Node = It.Node->Parent;
    }
  }

  return It;
}

BSTIteratorType BSTIterator_Next(BSTIteratorType It)
{
  if (It.BST == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    BSTIterator_Init(&It);
  }
  else if (It.Node == NULL)
  {
    // Cannot move past end.
    BSTIterator_Init(&It);
  }
  else
  {
    if (It.Node->Right != NULL)
    {
      It.Node = It.Node->Right;

      while (It.Node->Left != NULL)
      {
        It.Node = It.Node->Left;
      }
    }
    else
    {
      while (It.Node->Parent != NULL && It.Node->Parent->Right == It.Node)
      {
        It.Node = It.Node->Parent;
      }

      It.Node = It.Node->Parent;
    }
  }

  return It;
}

ConstBSTIteratorType BSTIterator_PrevConst(ConstBSTIteratorType It)
{
  if (It.BST == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    BSTIterator_InitConst(&It);
  }
  else if (It.Node == NULL)
  {
    // End case.
    if (It.BST->Root != NULL)
    {
      It.Node = It.BST->Root;

      while (It.Node->Right != NULL)
      {
        It.Node = It.Node->Right;
      }
    }
    else
    {
      BSTIterator_InitConst(&It);
    }
  }
  else
  {
    if (It.Node->Left != NULL)
    {
      It.Node = It.Node->Left;

      while (It.Node->Right != NULL)
      {
        It.Node = It.Node->Right;
      }
    }
    else
    {
      while (It.Node->Parent != NULL && It.Node->Parent->Left == It.Node)
      {
        It.Node = It.Node->Parent;
      }

      It.Node = It.Node->Parent;

      if (It.Node == NULL)
      {
        // Begin case.
        It.BST = NULL;
      }
    }
  }

  return It;
}

BSTIteratorType BSTIterator_Prev(BSTIteratorType It)
{
  if (It.BST == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    BSTIterator_Init(&It);
  }
  else if (It.Node == NULL)
  {
    // End case.
    if (It.BST->Root != NULL)
    {
      It.Node = It.BST->Root;

      while (It.Node->Right != NULL)
      {
        It.Node = It.Node->Right;
      }
    }
    else
    {
      BSTIterator_Init(&It);
    }
  }
  else
  {
    if (It.Node->Left != NULL)
    {
      It.Node = It.Node->Left;

      while (It.Node->Right != NULL)
      {
        It.Node = It.Node->Right;
      }
    }
    else
    {
      while (It.Node->Parent != NULL && It.Node->Parent->Left == It.Node)
      {
        It.Node = It.Node->Parent;
      }

      It.Node = It.Node->Parent;

      if (It.Node == NULL)
      {
        // Begin case.
        It.BST = NULL;
      }
    }
  }

  return It;
}

const void * BSTIterator_DataConst(ConstBSTIteratorType It)
{
  const void * Data = NULL;

  if (It.BST == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == NULL)
  {
    // Data cannot be acquired from End.
    // Do nothing.
  }
  else
  {
    Data = It.Node->Data;
  }

  return Data;
}

const void * BSTIterator_Data(BSTIteratorType It)
{
  const void * Data = NULL;

  if (It.BST == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (It.Node == NULL)
  {
    // Data cannot be acquired from End.
    // Do nothing.
  }
  else
  {
    Data = It.Node->Data;
  }

  return Data;
}

bool BSTIterator_EqualConst(ConstBSTIteratorType It1, ConstBSTIteratorType It2)
{
  bool Result = false;

  if (It1.BST == NULL || It2.BST == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Result = ((It1.BST == It2.BST) && (It1.Node == It2.Node));
  }

  return Result;
}

bool BSTIterator_Equal(BSTIteratorType It1, BSTIteratorType It2)
{
  bool Result = false;

  if (It1.BST == NULL || It2.BST == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else
  {
    Result = ((It1.BST == It2.BST) && (It1.Node == It2.Node));
  }

  return Result;
}

ConstBSTIteratorType BST_BeginConst(const BSTType * BST)
{
  ConstBSTIteratorType Begin;
  const BSTNodeType * Node = NULL;

  BSTIterator_InitConst(&Begin);

  if (BST == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    if (BST->Root != NULL)
    {
      Node = BST->Root;

      while (Node->Left != NULL)
      {
        Node = Node->Left;
      }
    }

    Begin.BST = BST;
    Begin.Node = Node;
  }

  return Begin;
}

BSTIteratorType BST_Begin(BSTType * BST)
{
  BSTIteratorType Begin;
  BSTNodeType * Node = NULL;

  BSTIterator_Init(&Begin);

  if (BST == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    if (BST->Root != NULL)
    {
      Node = BST->Root;

      while (Node->Left != NULL)
      {
        Node = Node->Left;
      }
    }

    Begin.BST = BST;
    Begin.Node = Node;
  }

  return Begin;
}

ConstBSTIteratorType BST_EndConst(const BSTType * BST)
{
  ConstBSTIteratorType End;

  BSTIterator_InitConst(&End);

  if (BST == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    End.BST = BST;
    End.Node = NULL;
  }

  return End;
}

BSTIteratorType BST_End(BSTType * BST)
{
  BSTIteratorType End;

  BSTIterator_Init(&End);

  if (BST == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else
  {
    End.BST = BST;
    End.Node = NULL;
  }

  return End;
}

/**
 * Access
 */

bool BST_Contains(const BSTType * BST, const void * Value)
{
  bool Result = false;

  BSTNodeType * Node = NULL;

  int32_t CompareResult = 0;

  if (BST == NULL || Value == NULL)
  {
    // NCL_E_NULL_PTR
    // Do nothing.
  }
  else if (BST->CompareFunction == NULL)
  {
    // NCL_E_INVALID_ARGUMENT
    // Do nothing.
  }
  else if (BST->Root == NULL)
  {
    // NCL_E_EMPTY
    // Do nothing.
  }
  else
  {
    Node = BST->Root;

    while (Node != NULL)
    {
      CompareResult = BST->CompareFunction(Value, Node->Data);

      if (CompareResult == 0)
      {
        Result = true;
        break;
      }
      else if (CompareResult < 0)
      {
        Node = Node->Left;
      }
      else if (CompareResult > 0)
      {
        Node = Node->Right;
      }
    }
  }

  return Result;
}

/**
 * Modification
 */

NCLResultType BST_Insert(BSTType * BST, const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  BSTNodeType ** Link = NULL;
  BSTNodeType * Parent = NULL;

  int32_t CompareResult = 0;

  if (BST == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (BST->CompareFunction == NULL)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else
  {
    Link = &BST->Root;

    while (*Link != NULL)
    {
      CompareResult = BST->CompareFunction(Value, (*Link)->Data);

      if (CompareResult == 0)
      {
        // A node with Data equal to Value is found.
        break;
      }
      else if (CompareResult < 0)
      {
        Parent = *Link;
        Link = &((*Link)->Left);
      }
      else if (CompareResult > 0)
      {
        Parent = *Link;
        Link = &((*Link)->Right);
      }
    }

    // Insert a new node, if the node with the given value does not exist.
    if ((*Link) == NULL)
    {
      *Link = BST_CreateNode(BST);

      if (*Link == NULL)
      {
        Result = NCL_E_ALLOC_FAILED;
      }
      else
      {
        // Node is successfully created. Copy data.
        memcpy((*Link)->Data, Value, BST->ElementSize);

        (*Link)->Parent = Parent;

        BST->Size++;
      }
    }
    else
    {
      // Duplicated Value.
      Result = NCL_E_ALREADY_EXISTS;
    }
  }

  return Result;
}

NCLResultType BST_Erase(BSTType * BST, const void * Value)
{
  NCLResultType Result = NCL_E_OK;

  BSTNodeType ** VictimLink = NULL;
  BSTNodeType * Victim = NULL;

  BSTNodeType ** SuccessorLink = NULL;
  BSTNodeType * Successor = NULL;

  int32_t CompareResult = 0;

  if (BST == NULL || Value == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else if (BST->CompareFunction == NULL)
  {
    Result = NCL_E_INVALID_ARGUMENT;
  }
  else if (BST->Root == NULL)
  {
    Result = NCL_E_EMPTY;
  }
  else
  {
    VictimLink = &BST->Root;

    while ((*VictimLink) != NULL)
    {
      CompareResult = BST->CompareFunction(Value, (*VictimLink)->Data);

      if (CompareResult == 0)
      {
        break;
      }
      else if (CompareResult < 0)
      {
        VictimLink = &((*VictimLink)->Left);
      }
      else if (CompareResult > 0)
      {
        VictimLink = &((*VictimLink)->Right);
      }
    }

    if ((*VictimLink) == NULL)
    {
      // Not found.
      Result = NCL_E_NOT_FOUND;
    }
    else
    {
      Victim = (*VictimLink);

      // Case: two children. Find the successor and transplant it.
      if ((Victim->Left != NULL) && (Victim->Right != NULL))
      {
        SuccessorLink = &Victim->Right;

        while ((*SuccessorLink)->Left != NULL)
        {
          SuccessorLink = &(*SuccessorLink)->Left;
        }

        Successor = *SuccessorLink;

        if (Successor == Victim->Right) // Successor->Parent == Victim.
        {
          // Translate Victim Left Subtree to Successor Left.
          Successor->Left = Victim->Left;

          if (Successor->Left)
          {
            Successor->Left->Parent = Successor;
          }
        }
        else
        {
          // Translate Successor Right to Successor Link / Successor Parent Left.
          *SuccessorLink = Successor->Right;

          if (*SuccessorLink)
          {
            (*SuccessorLink)->Parent = Successor->Parent;
          }

          // Translate Victim Left Subtree to Successor Left.
          Successor->Left = Victim->Left;

          if (Successor->Left)
          {
            Successor->Left->Parent = Successor;
          }

          // Translate Victim Right Subtree to Successor Right.
          Successor->Right = Victim->Right;

          if (Successor->Right)
          {
            Successor->Right->Parent = Successor;
          }
        }

        // Connect Successor to Victim Parent or Root.
        *VictimLink = Successor;

        // Connect Victim Parent to Successor.
        // Victim Parent is NULL, if Victim is Root.
        Successor->Parent = Victim->Parent;
      }
      else if (Victim->Left != NULL)
      {
        // Sucessor is Victim->Left.
        Successor = Victim->Left;

        // Connect Successor to Victim Parent or Root.
        *VictimLink = Successor;

        // Connect Victim Parent to Successor.
        // Victim Parent is NULL, if Victim is Root.
        Successor->Parent = Victim->Parent;
      }
      else if (Victim->Right != NULL)
      {
        // Sucessor is Victim->Right.
        Successor = Victim->Right;

        // Connect Successor to Victim Parent or Root.
        *VictimLink = Successor;

        // Connect Victim Parent to Successor
        // Victim Parent is NULL, if Victim is Root.
        Successor->Parent = Victim->Parent;
      }
      else
      {
        // Sucessor is NULL.
        Successor = NULL;

        // Connect Successor to Victim Parent or Root.
        *VictimLink = Successor;
      }

      BST_DeleteNode(Victim);
      BST->Size--;
    }
  }

  return Result;
}

/**
 * Edge Operations
 */

/**
 * Debug
 */

NCLResultType BST_Print(const BSTType * BST, PrintDataFunctionType PrintData)
{
  NCLResultType Result = NCL_E_OK;

  if (BST == NULL || PrintData == NULL)
  {
    Result = NCL_E_NULL_PTR;
  }
  else
  {
    printf("BST Data: \n");

    BST_PrintNode(BST->Root, PrintData);

    printf("\n");

    printf("BST->ElementSize       : %zu\n", BST->ElementSize);
    printf("BST->Size              : %zu\n", BST->Size);

    printf("\n");
  }

  return Result;
}
