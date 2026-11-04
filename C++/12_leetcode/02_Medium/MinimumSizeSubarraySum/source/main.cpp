/**
 * MinimumSizeSubarraySum
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

    int minSubArrayLen(int target, std::vector<int> & nums)
    {
      size_t left = 0;
      size_t right = 0;
      int sum = 0;
      size_t best_length = nums.size() + 1;

      while (right < nums.size())
      {
        sum += nums[right];
        right++;

        while (sum >= target)
        {
          best_length = std::min(best_length, right - left);
          sum -= nums[left];
          left++;
        }
      }

      best_length = (best_length == nums.size() + 1) ? 0 : best_length;

      return best_length;
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
