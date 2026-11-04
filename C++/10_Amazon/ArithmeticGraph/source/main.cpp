/**
 * Arithmetic Graph
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Node.hpp"

/**
 * y = (10 + 20) * (7 - 2) / 15
 *
 *               Division
 *               /      \
 *       Multiplication  15
 *        /          \
 *   Addition   Subtraction
 *   /     \      /      \
 *  10     20    7        2
 */

int main(int argc, const char * argv[])
{

  std::shared_ptr<TNode> n2 = std::make_shared<TNodeValue>(2.0);
  std::shared_ptr<TNode> n7 = std::make_shared<TNodeValue>(7.0);
  std::shared_ptr<TNode> n10 = std::make_shared<TNodeValue>(10.0);
  std::shared_ptr<TNode> n15 = std::make_shared<TNodeValue>(15.0);
  std::shared_ptr<TNode> n20 = std::make_shared<TNodeValue>(20.0);

  // (10 + 20)
  std::shared_ptr<TNode> add = std::make_shared<TNodeOperation>(TNodeOperation::EOperation::EAddition, n10, n20 );

  // (7 - 2)
  std::shared_ptr<TNode> sub = std::make_shared<TNodeOperation>(TNodeOperation::EOperation::ESubtraction, n7, n2 );

  // (10 + 20) * (7 - 2)
  std::shared_ptr<TNode> mul = std::make_shared<TNodeOperation>(TNodeOperation::EOperation::EMultiplication, add, sub);

  // ((10 + 20) * (7 - 2)) / 5
  std::shared_ptr<TNode> root = std::make_shared<TNodeOperation>(TNodeOperation::EOperation::EDivision, mul, n15);

  double result = root->Value();

  std::cout << "Result = " << result << std::endl;

  return 0;
}

