/**
 * String Stream
 *
   Sample Input:

   23,4,56

   Sample Output:

   23
   4
   56
 *
 */

#include <sstream>
#include <vector>
#include <iostream>

std::vector<int> parseInts(std::string str)
{
  std::vector<int> result;
	std::stringstream ss(str);
  char coma;
  int num;

  ss >> num;

  result.push_back(num);

  for (; ss >> coma >> num; )
  {
    result.push_back(num);
  }

  return result;
}

int main(int argc, const char * argv[])
{
  std::string str;
  std::cin >> str;
  std::vector<int> integers = parseInts(str);

  for(int i = 0; i < integers.size(); i++)
  {
    std::cout << integers[i] << "\n";
  }

  return 0;
}
