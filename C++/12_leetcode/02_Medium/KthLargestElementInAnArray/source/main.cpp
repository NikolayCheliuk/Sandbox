/**
 * Kth Largest Element in an Array
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

    int findKthLargest(std::vector<int> & nums, int k)
    {
      std::priority_queue<int, std::vector<int>, std::greater<int>> pq;

      for (size_t i = 0; i < nums.size(); i++)
      {
        if (pq.size() < k)
        {
          pq.push(nums[i]);
        }
        else
        {
          if (nums[i] > pq.top())
          {
            pq.pop();
            pq.push(nums[i]);
          }
        }
      }

      return pq.top();
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
