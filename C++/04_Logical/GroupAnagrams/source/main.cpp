/**
 * Group Anagrams
 * Group words that are anagrams of each other.
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

std::vector<std::vector<std::string>> GroupAnagrams(const std::vector<std::string> & data);

int main(int argc, const char * argv[])
{
  std::vector<std::string> data { "eat", "tea", "tan", "ate", "nat", "bat" };
  std::vector<std::vector<std::string>> result;

  result = GroupAnagrams(data);

  std::cout << "task7 result is: [";

  for (size_t i = 0; i < result.size(); i++)
  {
    if (i != 0)
    {
      std::cout << ", ";
    }

    std::cout << "[";

    for (size_t j = 0; j < result[i].size(); j++)
    {
      if (j != 0)
      {
        std::cout << ", ";
      }

      std::cout << result[i][j];
    }

    std::cout << "]";
  }

  std::cout << "]" << std::endl;

  return 0;
}

std::vector<std::vector<std::string>> GroupAnagrams(const std::vector<std::string> & data)
{
  std::vector<std::vector<std::string>> result;
  std::unordered_map<std::string, std::vector<std::string>> anagrams;

  for (auto it = data.begin(); it != data.end(); it++)
  {
    std::string key = *it;
    sort(key.begin(), key.end());
    anagrams[key].push_back(*it);
  }

  for (auto it = anagrams.begin(); it != anagrams.end(); it++)
  {
      result.push_back(it->second);
  }

  return result;
}