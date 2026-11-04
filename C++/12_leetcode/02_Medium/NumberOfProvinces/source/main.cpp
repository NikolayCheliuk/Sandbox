/**
 * Number Of Provinces
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

    int findCircleNum(const std::vector<std::vector<int>> & grid)
    {
      std::vector<bool> visited(grid.size(), false);
      int province_counter = 0;

      for (int i = 0; i < grid.size(); i++)
      {
        if (visited[i] == false)
        {
          province_counter++;
          bfs(grid, visited, i);
        }
      }

      return province_counter;
    }

    void dfs(const std::vector<std::vector<int>> & grid, std::vector<bool> & visited, int i)
    {
      visited[i] = true;

      for (int j = 0; j < grid[i].size(); j++)
      {
        if (grid[i][j] == 1 && visited[j] == false)
        {
          dfs(grid, visited, j);
        }
      }
    }

    void bfs(const std::vector<std::vector<int>>& grid, std::vector<bool>& visited, int i)
    {
      std::queue<int> q;

      visited[i] = true;
      q.push(i);

      while (!q.empty())
      {
        int i = q.front();
        q.pop();

        for (int j = 0; j < grid[i].size(); j++)
        {
          if (grid[i][j] == 1 && visited[j] == false)
          {
            visited[j] = true;
            q.push(j);
          }
        }
      }
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
