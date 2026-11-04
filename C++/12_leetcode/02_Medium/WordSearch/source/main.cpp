/**
 * WordSearch
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

    bool exist(std::vector<std::vector<char>> & board, const std::string & word)
    {
      bool result = false;
      int m = board.size();

      if (word.size() == 0)
      {
        return true;
      }

      for (int i = 0; i < m; i++)
      {
        int n = board[i].size();

        for (int j = 0; j < n; j++)
        {
          if (board[i][j] == word[0])
          {
            result = result || dfs(board, word, i, j, 0);
          }
        }
      }

      return result;
    }

    bool dfs(std::vector<std::vector<char>> & board, const std::string & word, int i, int j, int k)
    {
      bool result = false;
      char bkp = 0;

      bkp = board[i][j];
      board[i][j] = 0;

      if (k < word.size() - 1)
      {
        if (i > 0 && board[i - 1][j] == word[k + 1])
        {
          result = dfs(board, word, i - 1, j, k + 1);

          if (result == true)
          {
            return true;
          }
        }

        if (i < board.size() - 1 && board[i + 1][j] == word[k + 1])
        {
          result = dfs(board, word, i + 1, j, k + 1);

          if (result == true)
          {
            return true;
          }
        }

        if (j > 0 && board[i][j - 1] == word[k + 1])
        {
          result = dfs(board, word, i, j - 1, k + 1);

          if (result == true)
          {
            return true;
          }
        }

        if (j < board[i].size() - 1 && board[i][j + 1] == word[k + 1])
        {
          result = dfs(board, word, i, j + 1, k + 1);

          if (result == true)
          {
            return true;
          }
        }
      }
      else
      {
        result = true;
      }

      board[i][j] = bkp;

      return result;
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
