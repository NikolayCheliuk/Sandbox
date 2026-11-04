/**
 * RottingOranges
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

class Solution1
{
  private:

    struct Rott
    {
      size_t i;
      size_t j;
    };

  public:

    int orangesRotting(std::vector<std::vector<int>> & grid)
    {
      size_t m = 0;
      size_t n = 0;
      std::queue<Rott> rotts;
      int counter = 0;
      int rotted = 0;

      m = grid.size();

      for (size_t i = 0; i < m; i++)
      {
        n = grid[i].size();

        for (size_t j = 0; j < n; j++)
        {
          if (grid[i][j] == 2)
          {
            rotts.push({i, j});
          }
        }
      }

      while (!rotts.empty())
      {
        size_t layer_size = rotts.size();
        rotted = 0;

        for (size_t k = 0; k < layer_size; k++)
        {
          Rott rott = rotts.front();
          rotts.pop();
          bfs(grid, rott, rotts, rotted);
        }

        counter += rotted;
      }

      for (size_t i = 0; i < grid.size(); i++)
      {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
          if (grid[i][j] == 1)
          {
            return -1;
          }
        }
      }

      return counter;
    }

  private:

    void bfs(std::vector<std::vector<int>> & grid, const Rott & rott, std::queue<Rott> & rotts, int & rotted)
    {
      size_t i = rott.i;
      size_t j = rott.j;

      if (i > 0 && grid[i - 1][j] == 1)
      {
        grid[i - 1][j] = 2;
        rotts.push({i - 1, j});
        rotted = 1;
      }

      if (i < grid.size() - 1 && grid[i + 1][j] == 1)
      {
        grid[i + 1][j] = 2;
        rotts.push({i + 1, j});
        rotted = 1;
      }

      if (j > 0 && grid[i][j - 1] == 1)
      {
        grid[i][j - 1] = 2;
        rotts.push({i, j - 1});
        rotted = 1;
      }

      if (j < grid[i].size() - 1 && grid[i][j + 1] == 1)
      {
        grid[i][j + 1] = 2;
        rotts.push({i, j + 1});
        rotted = 1;
      }
    }
};

class Solution2
{
  private:

    struct Rott
    {
      size_t i;
      size_t j;
    };

  public:

    int orangesRotting(std::vector<std::vector<int>> & grid)
    {
      size_t m = 0;
      size_t n = 0;
      std::queue<Rott> rotts;
      int fresh = 0;
      int minutes = 0;

      m = grid.size();

      for (size_t i = 0; i < m; i++)
      {
        n = grid[i].size();

        for (size_t j = 0; j < grid[i].size(); j++)
        {
          if (grid[i][j] == 2)
          {
            rotts.push({i, j});
          }
          else if (grid[i][j] == 1)
          {
            fresh++;
          }
        }
      }

      while (!rotts.empty() && fresh > 0)
      {
        size_t layer_size = rotts.size();

        for (size_t k = 0; k < layer_size; k++)
        {
          Rott rott = rotts.front();
          rotts.pop();
          bfs(grid, rott, rotts, fresh);
        }

        minutes++;
      }

      return fresh == 0 ? minutes : -1;
    }

  private:

    void bfs(std::vector<std::vector<int>> & grid, const Rott & rott, std::queue<Rott> & rotts, int & fresh)
    {
      size_t i = rott.i;
      size_t j = rott.j;

      if (i > 0 && grid[i - 1][j] == 1)
      {
        grid[i - 1][j] = 2;
        rotts.push({i - 1, j});
        fresh--;
      }

      if (i + 1 < grid.size() && grid[i + 1][j] == 1)
      {
        grid[i + 1][j] = 2;
        rotts.push({i + 1, j});
        fresh--;
      }

      if (j > 0 && grid[i][j - 1] == 1)
      {
        grid[i][j - 1] = 2;
        rotts.push({i, j - 1});
        fresh--;
      }

      if (j + 1 < grid[i].size() && grid[i][j + 1] == 1)
      {
        grid[i][j + 1] = 2;
        rotts.push({i, j + 1});
        fresh--;
      }
    }
};

class Solution
{
  public:

    int orangesRotting(std::vector<std::vector<int>> & grid)
    {
      Solution1 solution1;
      Solution2 solution2;

      return solution2.orangesRotting(grid);
    }
};

int main(int argc, const char * argv[])
{
  Solution solution;

  {
    std::vector<std::vector<int>> grid =
    {
      {2, 1, 1},
      {1, 1, 0},
      {0, 1, 1}
    };

    std::cout << "Test 1: "
              << solution.orangesRotting(grid)
              << " (expected 4)\n";
  }

  {
    std::vector<std::vector<int>> grid =
    {
      {2, 1, 1},
      {0, 1, 1},
      {1, 0, 1}
    };

    std::cout << "Test 2: "
              << solution.orangesRotting(grid)
              << " (expected -1)\n";
  }

  {
    std::vector<std::vector<int>> grid =
    {
      {0, 2}
    };

    std::cout << "Test 3: "
              << solution.orangesRotting(grid)
              << " (expected 0)\n";
  }

  {
    std::vector<std::vector<int>> grid =
    {
      {2}
    };

    std::cout << "Test 4: "
              << solution.orangesRotting(grid)
              << " (expected 0)\n";
  }

  {
    std::vector<std::vector<int>> grid =
    {
      {1}
    };

    std::cout << "Test 5: "
              << solution.orangesRotting(grid)
              << " (expected -1)\n";
  }

  return 0;
}
