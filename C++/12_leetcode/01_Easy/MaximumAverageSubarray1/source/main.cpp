/**
 * MaximumAverageSubarray1
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

    double findMaxAverage(std::vector<int> & nums, int k)
    {
      int curr_sum = 0;
      int best_sum = 0;

      if (nums.size() < k)
      {
        // Error
      }
      else
      {
        for (size_t i = 0; i < k; i++)
        {
          curr_sum += nums[i];
        }

        best_sum = curr_sum;

        for (size_t i = k; i < nums.size(); i++)
        {
          curr_sum = curr_sum - nums[i - k] + nums[i];

          if (curr_sum > best_sum)
          {
            best_sum = curr_sum;
          }
        }
      }

      return static_cast<double>(best_sum) / k;;
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
