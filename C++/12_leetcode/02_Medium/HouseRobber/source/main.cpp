/**
 * House Robber
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

    int rob(std::vector<int> & nums)
    {
      int current = 0;
      int minus_1 = 0;
      int minus_2 = 0;

      for (size_t i = 0; i < nums.size(); i++)
      {
        current = std::max(minus_1, minus_2 + nums[i]);
        minus_2 = minus_1;
        minus_1 = current;
      }

      return current;
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
