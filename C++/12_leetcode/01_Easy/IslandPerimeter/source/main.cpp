/**
 * IslandPerimeter
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

    int islandPerimeter(std::vector<std::vector<int>> & grid)
    {
      int island_perimeter = 0;

      for (size_t i = 0; i < grid.size(); i++)
      {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
          if (grid[i][j] == 1)
          {
            if (i == 0 || grid[i - 1][j] == 0)
            {
              island_perimeter++;
            }

            if (i == grid.size() - 1 || grid[i + 1][j] == 0)
            {
              island_perimeter++;
            }

            if (j == 0 || grid[i][j - 1] == 0)
            {
              island_perimeter++;
            }

            if (j == grid[i].size() - 1 || grid[i][j + 1] == 0)
            {
              island_perimeter++;
            }
          }
        }
      }

      return island_perimeter;
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
