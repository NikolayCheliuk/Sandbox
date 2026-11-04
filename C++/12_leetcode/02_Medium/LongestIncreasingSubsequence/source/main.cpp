/**
 * LongestIncreasingSubsequence
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

class Solution1
{
  public:

    int lengthOfLIS(std::vector<int> & nums)
    {
      size_t result = 0;
      size_t n = nums.size();
      std::vector<size_t> lengths(n, 1);

      if (n == 0)
      {
        return 0;
      }

      for (size_t i = 0; i < n; i++)
      {
        for (size_t j = 0; j < i; j++)
        {
          if (nums[j] < nums[i])
          {
            lengths[i] = std::max(lengths[i], lengths[j] + 1);
          }
        }

        result = std::max(result, lengths[i]);
      }

      return result;
    }
};

class Solution2
{
  public:

    int lengthOfLIS(std::vector<int> & nums)
    {
      size_t n = nums.size();
      std::vector<int> tails;
      tails.reserve(n);

      if (n == 0)
      {
        return 0;
      }

      for (size_t i = 0; i < n; i++)
      {
        int x = nums[i];

        if (tails.empty() || x > tails.back())
        {
          tails.push_back(x);
        }
        else
        {
          auto it = std::lower_bound(tails.begin(), tails.end(), x);
          *it = x;
        }
      }

      return static_cast<int>(tails.size());
    }
};

class Solution
{
  public:

    int lengthOfLIS(std::vector<int> & nums)
    {
      Solution1 solution1;
      Solution2 solution2;

      return solution2.lengthOfLIS(nums);
    }
};

int main(int argc, const char * argv[])
{
  Solution solution;

  {
    std::vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};
    std::cout << "Test 1 (expected 4): "
              << solution.lengthOfLIS(nums) << std::endl;
  }

  {
    std::vector<int> nums = {0, 1, 0, 3, 2, 3};
    std::cout << "Test 2 (expected 4): "
              << solution.lengthOfLIS(nums) << std::endl;
  }

  {
    std::vector<int> nums = {7, 7, 7, 7, 7};
    std::cout << "Test 3 (expected 1): "
              << solution.lengthOfLIS(nums) << std::endl;
  }

  {
    std::vector<int> nums = {};
    std::cout << "Test 4 (expected 0): "
              << solution.lengthOfLIS(nums) << std::endl;
  }

  {
    std::vector<int> nums = {1};
    std::cout << "Test 5 (expected 1): "
              << solution.lengthOfLIS(nums) << std::endl;
  }

  return 0;
}
