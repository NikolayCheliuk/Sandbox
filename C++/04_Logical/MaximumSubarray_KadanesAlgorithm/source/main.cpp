/**
 * Maximum Subarray / Kadane’s Algorithm
 * Find the contiguous subarray with the largest sum.
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

std::vector<int32_t> MaximumSubarray(const std::vector<int32_t> & data)
{
  std::vector<int32_t> result;
  int32_t current_sum = 0;
  int32_t best_sum = 0;
  size_t current_left = 0;
  size_t current_right = 0;
  size_t best_left = 0;
  size_t best_right = 0;

  if (data.empty())
  {
    // Error
  }
  else
  {
    current_sum = data[0];
    best_sum =  data[0];

    for (size_t i = 1; i < data.size(); i++)
    {
      if (current_sum + data[i] < data[i])
      {
        current_left = i;
        current_right = i;
        current_sum = data[i];
      }
      else
      {
        current_right = i;
        current_sum += data[i];
      }

      if (current_sum > best_sum)
      {
        best_left = current_left;
        best_right = current_right;
        best_sum = current_sum;
      }
    }

    result = std::vector<int32_t>(data.begin() + best_left, data.begin() + best_right + 1);
  }

  return result;
}

int main(int argc, const char * argv[])
{
  std::vector<int32_t> data { -2, 1, -3, 4, -1, 2, 1, -5, 4 };
  std::vector<int32_t> result;

  result = MaximumSubarray(data);

  std::cout << "task8 result is: [";

  for (size_t i = 0; i < result.size(); i++)
  {
    if (i != 0)
    {
      std::cout << ", ";
    }

    std::cout << result[i];
  }

  std::cout << "]" << std::endl;

  return 0;
}
