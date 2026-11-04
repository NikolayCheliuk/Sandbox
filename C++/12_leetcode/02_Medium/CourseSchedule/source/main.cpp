/**
 * Course Schedule
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

    bool canFinish(int numCourses, std::vector<std::vector<int>> & prerequisites)
    {
      std::vector<std::vector<int>> in_dependency_lists(numCourses);
      std::vector<int>              out_dependency_list(numCourses);
      std::queue<int>               courses_queue;

      for (size_t i = 0; i < prerequisites.size(); i++)
      {
        int a = prerequisites[i][0];
        int b = prerequisites[i][1];

        in_dependency_lists[b].push_back(a);
        out_dependency_list[a]++;
      }

      for (size_t i = 0; i < numCourses; i++)
      {
        if (out_dependency_list[i] == 0)
        {
          courses_queue.push(i);
        }
      }

      size_t visited_course_count = 0;

      while (!courses_queue.empty())
      {
        int cur = courses_queue.front();
        courses_queue.pop();
        visited_course_count++;

        for (size_t i = 0; i < in_dependency_lists[cur].size(); i++)
        {
          int depending_one = in_dependency_lists[cur][i];
          out_dependency_list[depending_one]--;

          if (out_dependency_list[depending_one] == 0)
          {
            courses_queue.push(depending_one);
          }
        }
      }

      return visited_course_count == numCourses;
    }
};

int main(int argc, const char * argv[])
{
  return 0;
}
