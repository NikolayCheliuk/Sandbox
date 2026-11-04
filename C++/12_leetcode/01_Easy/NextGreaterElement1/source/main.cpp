/**
 * NextGreaterElement1
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

    std::vector<int> nextGreaterElement(std::vector<int> & nums1, std::vector<int> & nums2)
    {
      std::vector<int> result(nums1.size(), 0);
      std::stack<int> st;
      std::unordered_map<int, int> mp;

      mp.reserve(nums2.size());

      for (int x : nums2)
      {
        while (!st.empty() && x > st.top())
        {
          int p = st.top();
          st.pop();
          mp[p] = x;
        }

        st.push(x);
      }

      while (!st.empty())
      {
        int x = st.top();
        st.pop();
        mp[x] = -1;
      }

      for (size_t i = 0; i < nums1.size(); i++)
      {
        result[i] = mp[nums1[i]];
      }

      return result;
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
