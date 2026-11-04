/**
 * CoinChange
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

    int coinChange(std::vector<int> & coins, int amount)
    {
      std::vector<int> dp(amount + 1, amount + 1);

      dp[0] = 0;

      for (int x = 1; x < amount + 1; x ++)
      {
        for (size_t i = 0; i < coins.size(); i++)
        {
          int c = coins[i];

          if (x >= c)
          {
            dp[x] = std::min(dp[x], dp[x - c] + 1);
          }
        }
      }

      return (dp[amount] <= amount) ? dp[amount] : -1;
    }
};

int main(int argc, const char * argv[])
{

  Solution solution;

  {
    std::vector<int> coins = {1, 2, 5};
    int amount = 11;
    std::cout << "Test 1: expected 3, got "
              << solution.coinChange(coins, amount) << std::endl;
  }

  {
    std::vector<int> coins = {2};
    int amount = 3;
    std::cout << "Test 2: expected -1, got "
              << solution.coinChange(coins, amount) << std::endl;
  }

  {
    std::vector<int> coins = {2};
    int amount = 4;
    std::cout << "Test 3: expected 2, got "
              << solution.coinChange(coins, amount) << std::endl;
  }

  {
    std::vector<int> coins = {1};
    int amount = 0;
    std::cout << "Test 4: expected 0, got "
              << solution.coinChange(coins, amount) << std::endl;
  }

  return 0;
}
