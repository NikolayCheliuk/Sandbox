/**
 * Two Pointers (Sorted Array / Target Sum)
 * Use two pointers to find pairs in a sorted array that meet a condition (e.g., sum to target).
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

std::vector<std::pair<size_t, size_t>> FindTargetPairs(const std::vector<int32_t> & data, int32_t target);

int main(int argc, const char * argv[])
{
  std::vector<int32_t> data { 1, 2, 3, 4, 5, 6 };
  int32_t target { 7 };
  std::vector<std::pair<size_t, size_t>> result;

  result = FindTargetPairs(data, target);

  for (int32_t i = 0; i < result.size(); i++)
  {
    std::cout << "task2 result is " << result[i].first << " " << result[i].second << std::endl;
  }

  return 0;
}

std::vector<std::pair<size_t, size_t>> FindTargetPairs(const std::vector<int32_t> & data, int32_t target)
{
  std::vector<std::pair<size_t, size_t>> result;
  size_t left = 0;
  size_t right = data.size() - 1;

  while (right > left)
  {
    if (data[left] + data[right] == target)
    {
      result.push_back(std::make_pair(left, right));
      left++;
      right--;

      continue;
    }

    if (data[left] + data[right] < target)
    {
      left++;
      continue;
    }

    if(data[left] + data[right] > target)
    {
      right--;
      continue;
    }
  }

  return result;
}
