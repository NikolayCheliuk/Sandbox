/**
 * Node.cpp
 */

#include "Node.hpp"

TNode::~TNode()
{

}

TNodeValue::TNodeValue(double Value) :
  _value(Value)
{

}

TNodeValue::~TNodeValue()
{

}

double TNodeValue::Value()
{
  return _value;
}

TNodeOperation::TNodeOperation(EOperation operation, std::shared_ptr<TNode> left, std::shared_ptr<TNode> right) :
  _operation(operation),
  _left(left),
  _right(right)
{

}

TNodeOperation::~TNodeOperation()
{

}

double TNodeOperation::Value()
{
  double result = 0;

  switch (_operation)
  {
    case TNodeOperation::EOperation::EAddition:
    {
      result = _left->Value() + _right->Value();
      break;
    }

    case TNodeOperation::EOperation::ESubtraction:
    {
      result = _left->Value() - _right->Value();
      break;
    }

    case TNodeOperation::EOperation::EMultiplication:
    {
      result = _left->Value() * _right->Value();
      break;
    }

    case TNodeOperation::EOperation::EDivision:
    {
      result = _left->Value() / _right->Value();
      break;
    }

    default:
    {
      // Handle Error
      break;
    }
  }

  return result;
}
