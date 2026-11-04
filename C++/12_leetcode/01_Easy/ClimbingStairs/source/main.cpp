/**
 * Climbing Stairs
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
  private:

  class SolutionIteration
  {
    public:

      int climbStairs(int n)
      {
        switch (n)
        {
          case 1:
          {
            return  1;
          }

          case 2:
          {
            return  2;
          }

          default:
          {
            int p2 = 1;
            int p1 = 2;
            int c  = 0;

            for (int i = 3; i <= n; i++)
            {
              c = p2 + p1;
              p2 = p1;
              p1 = c;
            }

            return c;
          }
        }
      }
  };

  class SolutionRecursion
  {
    public:

      int climbStairs(int n)
      {
        std::vector<int> mem(n + 1, -1);
        return climbStairs(n, mem);
      }

    private:

      int climbStairs(int n, std::vector<int> & mem)
      {
        if (mem[n] == -1)
        {
          switch (n)
          {
            case 1:
            {
              mem[1] = 1;
              break;
            }

            case 2:
            {
              mem[2] = 2;
              break;
            }

            default:
            {
              mem[n] = climbStairs(n - 2, mem) + climbStairs(n - 1, mem);
              break;
            }
          }
        }

        return mem[n];
      }
  };

  public:

    int climbStairs(int n)
    {
      SolutionIteration s1;
      SolutionRecursion s2;

      return s1.climbStairs(n);
    }
};

int main(int argc, const char * argv[])
{
  int n = 5;
  int result = -1;
  Solution solution;

  result = solution.climbStairs(n);

  std::cout << "Result is: " << result << std::endl;

  return 0;
}
