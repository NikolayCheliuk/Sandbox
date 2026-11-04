/**
 * PacificAtlanticWaterFlow
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

class SolutionBFS
{
  private:

    struct Point
    {
      int i;
      int j;
    };

  public:

    std::vector<std::vector<int>> pacificAtlantic(std::vector<std::vector<int>> & heights)
    {
      int m = heights.size();
      int n = heights[0].size();

      std::vector<std::vector<int>> result;
      std::vector<std::vector<bool>> pacific(m, std::vector<bool>(n, false));
      std::vector<std::vector<bool>> atlantic(m, std::vector<bool>(n, false));

      std::queue<Point> qp;
      std::queue<Point> qa;

      // Pacific Left Side
      for (int i = 0; i < m; i++)
      {
        pacific[i][0] = true;
        qp.push({i, 0});
      }

      // Pacific Top Side
      for (int j = 0; j < n; j++)
      {
        pacific[0][j] = true;
        qp.push({0, j});
      }

      // Atlantic Right Side
      for (int i = 0; i < m; i++)
      {
        atlantic[i][n - 1] = true;
        qa.push({i, n - 1});
      }

      // Atlantic Bottom Side
      for (int j = 0; j < n; j++)
      {
        atlantic[m - 1][j] = true;
        qa.push({m - 1, j});
      }

      bfs(heights, pacific, qp);
      bfs(heights, atlantic, qa);

      // Comparison
      for (int i = 0; i < m; i++)
      {
        for (int j = 0; j < n; j++)
        {
          if (pacific[i][j] && atlantic[i][j])
          {
            result.push_back({i, j});
          }
        }
      }

      return result;
    }

    void bfs(const std::vector<std::vector<int>> & heights,
             std::vector<std::vector<bool>> & side,
             std::queue<Point> & q)
    {
      int m = heights.size();
      int n = heights[0].size();

      while (!q.empty())
      {
        Point p = q.front();
        q.pop();

        int i = p.i;
        int j = p.j;

        // Up
        if ((i > 0) &&
            (side[i - 1][j] == false) &&
            (heights[i - 1][j] >= heights[i][j]))
        {
          side[i - 1][j] = true;
          q.push({i - 1, j});
        }

        // Down
        if ((i < m - 1) &&
            (side[i + 1][j] == false) &&
            (heights[i + 1][j] >= heights[i][j]))
        {
          side[i + 1][j] = true;
          q.push({i + 1, j});
        }

        // Left
        if ((j > 0) &&
            (side[i][j - 1] == false) &&
            (heights[i][j - 1] >= heights[i][j]))
        {
          side[i][j - 1] = true;
          q.push({i, j - 1});
        }

        // Right
        if ((j < n - 1) &&
            (side[i][j + 1] == false) &&
            (heights[i][j + 1] >= heights[i][j]))
        {
          side[i][j + 1] = true;
          q.push({i, j + 1});
        }
      }
    }
};

class SolutionDFS
{
  private:

    struct Point
    {
      int i;
      int j;
    };

  public:

    std::vector<std::vector<int>> pacificAtlantic(std::vector<std::vector<int>> & heights)
    {
      int m = heights.size();
      int n = heights[0].size();

      std::vector<std::vector<int>> result;
      std::vector<std::vector<bool>> pacific(m, std::vector(n, false));
      std::vector<std::vector<bool>> atlantic(m, std::vector(n, false));

      // Pacific Left Side
      for (int i = 0; i < m; i++)
      {
        if (pacific[i][0] == false)
        {
          dfs(heights, i, 0, pacific);
        }
      }

      // Pacific Top Side
      for (int j = 0; j < n; j++)
      {
        if (pacific[0][j] == false)
        {
          dfs(heights, 0, j, pacific);
        }
      }

      // Atlantic Right Side
      for (int i = 0; i < m; i++)
      {
        if (atlantic[i][n - 1] == false)
        {
          dfs(heights, i, n - 1, atlantic);
        }
      }

      // Atlantic Bottom Side
      for (int j = 0; j < n; j++)
      {
        if (atlantic[m - 1][j] == false)
        {
          dfs(heights, m - 1, j, atlantic);
        }
      }

      // Comparison
      for (int i = 0; i < m; i++)
      {
        for (int j = 0; j < n; j++)
        {
          if (pacific[i][j] == 1 && atlantic[i][j] == 1)
          {
            result.push_back({i, j});
          }
        }
      }

      return result;
    }

    void dfs(const std::vector<std::vector<int>> & heights, int i, int j, std::vector<std::vector<bool>> & side)
    {
      side[i][j] = true;

      // Up
      if ((i > 0) &&
          (heights[i - 1][j] >= heights[i][j]) &&
          (side[i - 1][j] == false))
      {
        dfs(heights, i - 1, j, side);
      }

      // Down
      if ((i < heights.size() - 1) &&
          (heights[i + 1][j] >= heights[i][j]) &&
          (side[i + 1][j] == false))
      {
        dfs(heights, i + 1, j, side);
      }

      // Left
      if ((j > 0) &&
          (heights[i][j - 1] >= heights[i][j]) &&
          (side[i][j - 1] == false))
      {
        dfs(heights, i, j - 1, side);
      }

      // Right
      if ((j < heights[i].size() - 1) &&
          (heights[i][j + 1] >= heights[i][j]) &&
          (side[i][j + 1] == false))
      {
        dfs(heights, i, j + 1, side);
      }
    }
};

class Solution
{
  public:

    std::vector<std::vector<int>> pacificAtlantic(std::vector<std::vector<int>> & heights)
    {
      SolutionBFS bfs;
      SolutionDFS dfs;

      return bfs.pacificAtlantic(heights);
    }
};

int main(int argc, const char * argv[])
{
  std::vector<std::vector<int>> heights =
  {
    {1, 2, 2, 3, 5},
    {3, 2, 3, 4, 4},
    {2, 4, 5, 3, 1},
    {6, 7, 1, 4, 5},
    {5, 1, 1, 2, 4}
  };

  Solution solution;
  std::vector<std::vector<int>> result = solution.pacificAtlantic(heights);

  std::cout << "Result:\n";
  for (const auto & p : result)
  {
    std::cout << "[" << p[0] << ", " << p[1] << "]\n";
  }

  return 0;
}
