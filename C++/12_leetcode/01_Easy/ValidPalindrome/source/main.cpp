/**
 * ValidPalindrome
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

    bool isPalindrome(const std::string & s)
    {
      if (s.empty())
      {
        return true;
      }

      size_t left = 0;
      size_t right = s.size() - 1;

      while (left < right)
      {
        if (left < right && !std::isalnum(static_cast<unsigned char>(s[left])))
        {
          left++;
          continue;
        }

        if (left < right && !std::isalnum(static_cast<unsigned char>(s[right])))
        {
          right--;
          continue;
        }

        if (std::tolower(static_cast<unsigned char>(s[left])) != std::tolower(static_cast<unsigned char>(s[right])))
        {
          return false;
        }

        left++;
        right--;
      }

      return true;
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
