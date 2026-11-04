/**
 * ReversePolishNotation
 */

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Solution
{
  public:

    int evalRPN(std::vector<std::string> & tokens)
    {
      std::stack<int> st;

      for (auto & token : tokens)
      {
        if (token == "+")
        {
          int right = st.top(); st.pop();
          int left = st.top(); st.pop();
          st.push(left + right);
          continue;
        }

        if (token == "-")
        {
          int right = st.top(); st.pop();
          int left = st.top(); st.pop();
          st.push(left - right);
          continue;
        }

        if (token == "*")
        {
          int right = st.top(); st.pop();
          int left = st.top(); st.pop();
          st.push(left * right);
          continue;
        }

        if (token == "/")
        {
          int right = st.top(); st.pop();
          int left = st.top(); st.pop();
          st.push(left / right);
          continue;
        }

        st.push(std::stoi(token));
      }

      return st.top();
    }
};

int main(int argc, const char * argv[])
{

  Solution solution;

  {
    std::vector<std::string> tokens = {"2","1","+","3","*"};
    std::cout << "Result 1 = " << solution.evalRPN(tokens) << " (expected 9)\n";
  }

  {
    std::vector<std::string> tokens = {"4","13","5","/","+"};
    std::cout << "Result 2 = " << solution.evalRPN(tokens) << " (expected 6)\n";
  }

  {
    std::vector<std::string> tokens =
      {"10","6","9","3","+","-11","*","/","*","17","+","5","+"};
    std::cout << "Result 3 = " << solution.evalRPN(tokens) << " (expected 22)\n";
  }

  return 0;
}
