/**
 * C++11 Variadics
 */


#include <iostream>

template<typename T>
void print(const T& Value)
{
  std::cout << Value;
}

template<typename T, typename... Args>
void print(const T& first, const Args & ... rest)
{
  std::cout << first << " ";
  print(rest ...);
}

int main(int argc, const char * argv[])
{
  print(1, 2.5, "hello", 'A');
  std::cout << std::endl;
}
