/**
 * Number Of Islands
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

    int numIslands(std::vector<std::vector<char>> & grid)
    {
      int island_counter = 0;

      for (size_t i = 0; i < grid.size(); i++)
      {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
          if (grid[i][j] == '1')
          {
            island_counter++;
            dfs(grid, i, j);
          }
        }
      }

      return island_counter;
    }

    void dfs(std::vector<std::vector<char>> & grid, int i, int j)
    {
      grid[i][j] = '0';

      if (i > 0 && grid[i - 1][j] == '1')
      {
        dfs(grid, i - 1, j);
      }

      if (i < grid.size() - 1 && grid[i + 1][j] == '1')
      {
        dfs(grid, i + 1, j);
      }

      if (j > 0 && grid[i][j - 1] == '1')
      {
        dfs(grid, i, j - 1);
      }

      if (j < grid[i].size() - 1 && grid[i][j + 1] == '1')
      {
        dfs(grid, i, j + 1);
      }
    }

    void bfs(std::vector<std::vector<char>> & grid, int i, int j)
    {
      std::queue<std::pair<int, int>> q;

      grid[i][j] = '0';
      q.push({i, j});

      while (!q.empty())
      {
        auto [x, y] = q.front();
        q.pop();

        if (x > 0 && grid[x - 1][y] == '1')
        {
          grid[x - 1][y] = '0';
          q.push({x - 1, y});
        }

        if (x < grid.size() - 1 && grid[x + 1][y] == '1')
        {
          grid[x + 1][y] = '0';
          q.push({x + 1, y});
        }

        if (y > 0 && grid[x][y - 1] == '1')
        {
          grid[x][y - 1] = '0';
          q.push({x, y - 1});
        }

        if (y < grid[0].size() - 1 && grid[x][y + 1] == '1')
        {
          grid[x][y + 1] = '0';
          q.push({x, y + 1});
        }
      }
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
