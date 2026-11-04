/**
 * Attribute Parser
 *
   Sample Input:

   4 3
   <tag1 Value = "HelloWorld">
   <tag2 name = "Name1">
   </tag2>
   </tag1>
   tag1.tag2~name
   tag1~name
   tag1~Value

   Sample Output:

   Name1
   Not Found!
   HelloWorld
 *
 */

#include <cmath>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <algorithm>

int main(int argc, const char * argv[])
{
  size_t N = 0;
  size_t Q = 0;

  std::vector<std::string> elements;
  std::unordered_map<std::string, std::string> attributes;

  std::cin >> N >> Q;
  std::cin.ignore();

  for (size_t i = 0; i < N; i++)
  {
    std::string line;

    std::getline(std::cin, line);

    if (line[1] == '/')
    {
      if (!elements.empty())
      {
        elements.pop_back();
      }
    }
    else
    {
      std::string tag;
      std::string attribute;
      std::string eq;
      std::string Value;

      line = line.substr(1, line.size() - 2);

      std::stringstream ss(line);

      ss >> tag;

      //std::cout << "tag = " << tag << std::endl;

      elements.push_back(tag);

      while (ss >> attribute >> eq >> Value)
      {
        Value = Value.substr(1, Value.size() - 2);

        //std::cout << attribute << " = " << Value << std::endl;

        std::string querry;

        for (size_t i = 0; i < elements.size(); i++)
        {
          if (!querry.empty())
          {
            querry += ".";
          }

          querry += elements[i];
        }

        querry += "~" + attribute;

        attributes[querry] = Value;

        //std::cout << querry << " = " << Value << std::endl;
      }
    }
  }

  for (size_t i = 0; i < Q; i++)
  {
    std::string querry;

    std::cin >> querry;

    if (attributes.find(querry) != attributes.end())
    {
      std::cout << attributes[querry] << std::endl;
    }
    else
    {
      std::cout << "Not Found!" << std::endl;
    }
  }

  return 0;
}
