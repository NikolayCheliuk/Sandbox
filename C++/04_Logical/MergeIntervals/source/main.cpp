/**
 * Merge Intervals
 * Given a list of intervals, merge all overlapping intervals.
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

std::vector<std::pair<int32_t, int32_t>> MergeIntervals(std::vector<std::pair<int32_t, int32_t>> & data);

int main(int argc, const char * argv[])
{
  std::vector<std::pair<int32_t, int32_t>> data { {1, 3}, {2, 6}, {8, 10}, {15, 18} };
  std::vector<std::pair<int32_t, int32_t>> result;

  result = MergeIntervals(data);

  for (size_t i = 0; i < result.size(); i++)
  {
    std::cout << "task6 result is " << result[i].first << ", " << result[i].second << std::endl;
  }

  return 0;
}

std::vector<std::pair<int32_t, int32_t>> MergeIntervals(std::vector<std::pair<int32_t, int32_t>> & data)
{
  std::vector<std::pair<int32_t, int32_t>> result;

  if (data.empty())
  {
    // Handle Error or do nothing
  }
  else
  {
    std::sort(
      data.begin(),
      data.end(),
      [](const std::pair<int32_t, int32_t> & a, const std::pair<int32_t, int32_t> & b)
      {
        return a.first < b.first;
      }
    );

    result.push_back(*data.begin());

    for (auto it = std::next(data.begin()); it != data.end(); it++)
    {
      auto last = std::prev(result.end());

      if (last->second >= it->first)
      {
        last->second = it->second;
      }
      else
      {
        result.push_back(*it);
      }
    }
  }

  return result;
}
