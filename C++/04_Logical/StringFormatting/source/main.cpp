/**
 * StringFormatting
 */

#include <iostream>
#include <string>

std::string formatString(const std::string& s)
{
  std::string result;
  int block_symbol_count = 0;

  // main loop: copy characters directly into result
  for (char c : s)
  {
    if (c == ' ' || c == '-')
    {
      continue;
    }

    if (block_symbol_count == 3)
    {
      result.push_back(' ');
      block_symbol_count = 0;
    }

    result.push_back(c);
    block_symbol_count++;
  }

  // if last block has only 1 character, fix spacing
  if (block_symbol_count == 1)
  {
    size_t pos = result.find_last_of(' ');

    if (pos != std::string::npos)
    {
      std::swap(result[pos - 1], result[pos]);
    }
  }

  result.push_back(';');

  return result;
}


int main(int argc, const char * argv[])
{
  // Example 1
  std::string s1 = "AA-44 BB 55CD 83FG";
  std::cout << formatString(s1) << std::endl; // expected: AA4 4BB 55C D83 FG;

  // Example 2
  std::string s2 = "00-44 48 5555 8361";
  std::cout << formatString(s2) << std::endl; // expected: 004 448 555 583 61;

  // Example 3
  std::string s3 = "0 - 22 1985--324";
  std::cout << formatString(s3) << std::endl; // expected: 022 198 53 24;

  // Example 4
  std::string s4 = "ABC372654";
  std::cout << formatString(s4) << std::endl; // expected: ABC 372 654;

  return 0;
}
