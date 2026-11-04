/**
 * Longest Substring Without Repeating Characters
 * Find the length of the longest substring without duplicate characters.
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

    std::string LengthOfLongestSubstring(std::string s)
    {
      std::string result;
      std::unordered_map<char, size_t> symbols;
      size_t left = 0;
      size_t right = 0;

      for (size_t i = 0; i < s.size(); i++)
      {
        if (symbols.find(s[i]) != symbols.end())
        {
          left = std::max(left, symbols[s[i]] + 1);
        }

        symbols[s[i]] = i;
        right++;

        if (right - left > result.size())
        {
          result = s.substr(left, right - left);
        }
      }

      return result;
    }
};

int main(int argc, const char * argv[])
{
  std::string result;
  std::string data = "abcabcbb";
  data = "abba";
  Solution solution;

  result = solution.LengthOfLongestSubstring(data);
  std::cout << "task5 result is " << result << std::endl;

  return 0;
}
