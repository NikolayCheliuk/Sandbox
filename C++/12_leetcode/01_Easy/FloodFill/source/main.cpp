/**
 * FloodFill
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

    std::vector<std::vector<int>> floodFill(std::vector<std::vector<int>> & image, int sr, int sc, int color_out)
    {
      int color_in = image[sr][sc];

      if (color_in == color_out)
      {
        return image;
      }

      dfs(image, sr, sc, color_in, color_out);

      return image;
    }

    void dfs(std::vector<std::vector<int>> & image, int i, int j, int color_in, int color_out)
    {
      image[i][j] = color_out;

      if (i > 0 && image[i - 1][j] == color_in)
      {
        dfs(image, i - 1, j, color_in, color_out);
      }

      if (i < image.size() - 1 && image[i + 1][j] == color_in)
      {
        dfs(image, i + 1, j, color_in, color_out);
      }

      if (j > 0 && image[i][j - 1] == color_in)
      {
        dfs(image, i, j - 1, color_in, color_out);
      }

      if (j < image[i].size() - 1 && image[i][j + 1] == color_in)
      {
        dfs(image, i, j + 1, color_in, color_out);
      }
    }

    void bfs(std::vector<std::vector<int>> & image, int i, int j, int color_in, int color_out)
    {
      std::queue<std::pair<int, int>> q;

      image[i][j] = color_out;
      q.push({i, j});

      while (!q.empty())
      {
        auto [x, y] = q.front();
        q.pop();

        if (x > 0 && image[x - 1][y] == color_in)
        {
          image[x - 1][y] = color_out;
          q.push({x - 1, y});
        }

        if (x < image.size() - 1 && image[x + 1][y] == color_in)
        {
          image[x + 1][y] = color_out;
          q.push({x + 1, y});
        }

        if (y > 0 && image[x][y - 1] == color_in)
        {
          image[x][y - 1] = color_out;
          q.push({x, y - 1});
        }

        if (y < image[0].size() - 1 && image[x][y + 1] == color_in)
        {
          image[x][y + 1] = color_out;
          q.push({x, y + 1});
        }
      }
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
