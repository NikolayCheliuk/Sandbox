/**
 * Valid Parentheses
 * Check if a string of brackets is valid (properly opened and closed).
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

bool match(char open, char close)
{
  return (open == '(' && close == ')') ||
         (open == '[' && close == ']') ||
         (open == '{' && close == '}');
}

bool CheckStringForBraces(const std::string & s)
{
  std::stack<char> st;

  for (auto c : s)
  {
    switch (c)
    {
      case '(':
      case '[':
      case '{':
      {
        st.push(c);
        break;
      }

      case ')':
      case ']':
      case '}':
      {
        if (st.empty() || !match(st.top(), c))
        {
          return false;
        }

        st.pop();

        break;
      }
    }
  }

  return st.empty();
}

int main(int argc, const char * argv[])
{
  std::string data = "{[()]}{}";
  bool result = false;

  result = CheckStringForBraces(data);

  std::cout << "task3 result is " << result << std::endl;

  return 0;
}
