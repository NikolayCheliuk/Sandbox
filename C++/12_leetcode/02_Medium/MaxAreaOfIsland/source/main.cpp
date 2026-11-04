/**
 * Max Area Of Island
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

    int maxAreaOfIsland(std::vector<std::vector<int>> & grid)
    {
      int island_square_max = 0;

      for (size_t i = 0; i < grid.size(); i++)
      {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
          if (grid[i][j] == 1)
          {
            int island_square = 0;

            dfs(grid, island_square, i, j);

            island_square_max = std::max(island_square_max, island_square);
          }
        }
      }

      return island_square_max;
    }

    void dfs(std::vector<std::vector<int>> & grid, int & island_square, int i, int j)
    {
      grid[i][j] = 0;
      island_square++;

      if (i > 0 && grid[i - 1][j] == 1)
      {
        dfs(grid, island_square, i - 1, j);
      }

      if (i < grid.size() - 1 && grid[i + 1][j] == 1)
      {
        dfs(grid, island_square, i + 1, j);
      }

      if (j > 0 && grid[i][j - 1] == 1)
      {
        dfs(grid, island_square, i, j - 1);
      }

      if (j < grid[i].size() - 1 && grid[i][j + 1] == 1)
      {
        dfs(grid, island_square, i, j + 1);
      }
    }

    void bfs(std::vector<std::vector<int>> & grid, int & island_square, int i, int j)
    {
      std::queue<std::pair<int, int>> q;

      grid[i][j] = 0;
      island_square++;
      q.push({i, j});

      while (!q.empty())
      {
        auto [x, y] = q.front();
        q.pop();

        if (x > 0 && grid[x - 1][y] == 1)
        {
          grid[x - 1][y] = 0;
          island_square++;
          q.push({x - 1, y});
        }

        if (x < grid.size() - 1 && grid[x + 1][y] == 1)
        {
          grid[x + 1][y] = 0;
          island_square++;
          q.push({x + 1, y});
        }

        if (y > 0 && grid[x][y - 1] == 1)
        {
          grid[x][y - 1] = 0;
          island_square++;
          q.push({x, y - 1});
        }

        if (y < grid[0].size() - 1 && grid[x][y + 1] == 1)
        {
          grid[x][y + 1] = 0;
          island_square++;
          q.push({x, y + 1});
        }
      }
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
