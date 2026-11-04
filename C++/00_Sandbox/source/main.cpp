/**
 * Sandbox
 */

#include <algorithm>
#include <cstdint>
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

void foo1(char array[16])
{
  printf("foo1()\n");

  for (size_t i = 0; i < 16; i++)
  {
    if (i > 0)
    {
      printf(", ");
    }

    printf("%c", array[i]);
  }

  printf("\n");
}

void foo2(char (*array)[16])
{
  printf("foo2()\n");

  for (size_t i = 0; i < 16; i++)
  {
    if (i > 0)
    {
      printf(", ");
    }

    printf("%c", (*array)[i]);
  }

  printf("\n");
}

struct A
{
  uint8_t a;
  uint32_t b;
  uint8_t c;
};

typedef union {
    uint32_t raw;   // доступ к целому регистру
    struct {
        uint32_t a : 1;
        uint32_t b : 8;
        uint32_t c : 1;
        uint32_t d : 8;
        uint32_t   : 14; // добиваем до 32 бит
    } bits;         // доступ к отдельным полям
} Reg;

template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
void foo(T)
{
  std::cout << "integral" << std::endl;
}

template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
void foo(T)
{
  std::cout << "floating" << std::endl;
}

int main(int argc, const char * argv[])
{
  char array[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

  foo1(array);
  foo2(&array);

  printf("A   - %zu bytes\n", sizeof(A));
  printf("Reg - %zu bytes\n", sizeof(Reg));

  foo(13);
  foo(3.14);

  return 0;
}
