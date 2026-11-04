/**
 * Two Sum
 * Find two numbers in an array that add up to a target Value.
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
  std::vector<std::pair<size_t, size_t>> result;
  std::vector<int32_t> data { 2, 7, 11, 15, 3, 6 };
  int32_t target { 9 };

  result = FindTargetPairs(data, target);

  for (int32_t i = 0; i < result.size(); i++)
  {
    std::cout << "task1 result is " << result[i].first << " " << result[i].second << std::endl;
  }

  return 0;
}

std::vector<std::pair<size_t, size_t>> FindTargetPairs(const std::vector<int32_t> & data, int32_t target)
{
  std::vector<std::pair<size_t, size_t>> result;
  std::unordered_map<int32_t, size_t> map;

  for (size_t i = 0; i < data.size(); i++)
  {
    int32_t diff = target - data[i];

    if (map.find(diff) == map.end())
    {
      map[data[i]] = i;
    }
    else
    {
      result.push_back(std::make_pair(map[diff], i));
    }
  }

  return result;
}
