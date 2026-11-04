/**
 * Node.hpp
 */

#pragma once

#include <memory>

class TNode
{
public:
  virtual ~TNode() = 0;
  virtual double Value() = 0;

private:

};

class TNodeValue :
  public TNode
{
public:
  TNodeValue(double Value);
  virtual ~TNodeValue();
  virtual double Value();

private:
  double _value;
};

class TNodeOperation :
  public TNode
{
public:
  enum class EOperation
  {
    EAddition,
    ESubtraction,
    EMultiplication,
    EDivision
  };

public:
  TNodeOperation(EOperation operation, std::shared_ptr<TNode> left, std::shared_ptr<TNode> right);
  virtual ~TNodeOperation();
  virtual double Value();

private:
  EOperation _operation;
  std::shared_ptr<TNode> _left;
  std::shared_ptr<TNode> _right;
};
