/**
 * DailyTemperatures
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

class Solution
{
  public:

    std::vector<int> dailyTemperatures(std::vector<int> & temperatures)
    {
      size_t n = temperatures.size();
      std::vector<int> result(n, 0);
      std::stack<size_t> st;

      for (size_t i = 0; i < n; i++)
      {
        while (!st.empty() && temperatures[i] > temperatures[st.top()])
        {
          size_t p = st.top();
          st.pop();
          result[p] = i - p;
        }

        st.push(i);
      }

      return result;
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
