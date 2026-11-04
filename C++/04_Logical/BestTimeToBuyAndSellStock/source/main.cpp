/**
 * Best Time to Buy and Sell Stock
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

std::pair<size_t, size_t> BestTimeToBuyAndSellStock(const std::vector<int32_t> & data);

int main(int argc, const char * argv[])
{
  std::vector<int32_t> data { 7, 1, 5, 3, 6, 4 };
  std::pair<size_t, size_t> result;

  result = BestTimeToBuyAndSellStock(data);

  std::cout << "task9 result is: [" << result.first << ", " << result.second << "]" << std::endl;

  return 0;
}

std::pair<size_t, size_t> BestTimeToBuyAndSellStock(const std::vector<int32_t> & data)
{
  std::pair<size_t, size_t> result;
  int32_t min_price = 0;
  int32_t best_delta = 0;
  size_t left = 0;
  size_t best_left = 0;
  size_t best_right = 0;

  if (data.empty())
  {
    // Error
  }
  else
  {
    min_price = data[0];

    for (size_t i = 1; i < data.size(); i++)
    {
      if (data[i] < min_price)
      {
        left = i;
        min_price = data[i];
      }

      if ((data[i] - min_price) > best_delta)
      {
        best_left = left;
        best_right = i;
        best_delta = data[i] - min_price;
      }
    }
  }

  if (best_left < best_right)
  {
    result = std::make_pair(best_left, best_right);
  }

  return result;
}
