/**
 * Filmography.cpp
 */

#include "Filmography.hpp"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> FindActorsForCharacter(const std::vector<Movie> & movies, const std::string & character_name)
{
  std::vector<std::string> result;

  for (const auto & movie : movies)
  {
    for (const auto & cast : movie._cast_list)
    {
      if (cast._character_name == character_name)
      {
        result.push_back(cast._name);
      }
    }
  }

  return result;
}

std::vector<std::pair<std::string, int>> TopActorsForCharacter(const std::vector<Movie>& movies, const std::string& character_name, int N)
{
  std::unordered_map<std::string, int> counter;

  // Frequency calculation
  for (const auto & movie : movies)
  {
    for (const auto & cast : movie._cast_list)
    {
      if (cast._character_name == character_name)
      {
          counter[cast._name]++;
      }
    }
  }

  // Make vector for sorting
  std::vector<std::pair<std::string, int>> freq(counter.begin(), counter.end());

  std::sort(
    freq.begin(),
    freq.end(),
    [](const auto& a, const auto& b)
    {
      return a.second > b.second;
    }
  );

  if (freq.size() > static_cast<size_t>(N))
  {
    freq.resize(N);
  }

  return freq;
}
