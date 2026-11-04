/**
 * Most Frequent Element / Top K Frequent Elements
 * Find the element that appears most frequently in an array.
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

int32_t FindTheMostFrequentElement(const std::vector<int32_t> & data);

std::vector<int32_t> FindTheMostFrequentElements(const std::vector<int32_t> & data);

int main(int argc, const char * argv[])
{
  std::vector<int32_t> data { 1, 3, 2, 3, 4, 3, 2, 2 };
  int32_t most_frequent_element = -1;
  std::vector<int32_t> most_frequent_elements;

  most_frequent_element = FindTheMostFrequentElement(data);
  std::cout << "task4 result1 is " << most_frequent_element << std::endl;

  most_frequent_elements = FindTheMostFrequentElements(data);

  for (size_t i = 0; i < most_frequent_elements.size(); i++)
  {
    std::cout << "task4 result2 is " << most_frequent_elements[i] << std::endl;
  }

  return 0;
}

int32_t FindTheMostFrequentElement(const std::vector<int32_t> & data)
{
  int32_t result = -1;
  int32_t max_frequency = 0;
  std::unordered_map<int32_t, int32_t> map_element_frequencies;

  for (size_t i = 0; i < data.size(); i++)
  {
    map_element_frequencies[data[i]]++;

    if (map_element_frequencies[data[i]] > max_frequency)
    {
      result = data[i];
      max_frequency = map_element_frequencies[data[i]];
    }
  }

  return result;
}

std::vector<int32_t> FindTheMostFrequentElements(const std::vector<int32_t> & data)
{
  std::vector<int32_t> result;
  int32_t max_frequency = 0;
  std::unordered_map<int32_t, int32_t> map_element_frequencies;

  for (size_t i = 0; i < data.size(); i++)
  {
    map_element_frequencies[data[i]]++;
  }

  for (auto it = map_element_frequencies.begin(); it != map_element_frequencies.end(); ++it)
  {
    if (it->second > max_frequency)
    {
      max_frequency = it->second;
    }
  }

  for (auto it = map_element_frequencies.begin(); it != map_element_frequencies.end(); ++it)
  {
    if (it->second == max_frequency)
    {
      result.push_back(it->first);
    }
  }

  return result;
}

