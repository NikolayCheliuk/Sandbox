/**
 * C++17 Variadics
 *
   Sample Input:
   2
   65 1
   10 0

   Sample Output:
   0100000000000000000000000000000000000000000000000000000000000000
   0000000000100000000000000000000000000000000000000000000000000000
 *
 */

#include <iostream>

// Enter your code for reversed_binary_value<bool...>()

#if 1

template<bool... bits>
int reversed_binary_value()
{
  int Value = 0;
  int weight = 1;
  ((Value += bits * weight, weight <<= 1), ...);
  return Value;
}

#else

template<bool... Bits>
struct reversed_binary_impl;

template<>
struct reversed_binary_impl<>
{
  static const int Value = 0;
};

template<bool B, bool... Rest>
struct reversed_binary_impl<B, Rest...>
{
  static const int Value = B + 2 * reversed_binary_impl<Rest...>::Value;
};

template<bool... Bits>
int reversed_binary_value()
{
    return reversed_binary_impl<Bits...>::Value;
}

#endif

template <int n, bool...digits>
struct CheckValues
{
	static void check(int x, int y)
	{
		CheckValues<n - 1, 0, digits...>::check(x, y);
		CheckValues<n - 1, 1, digits...>::check(x, y);
	}
};

template <bool...digits>
struct CheckValues<0, digits...>
{
  static void check(int x, int y)
  {
    int z = reversed_binary_value<digits...>();
    std::cout << (z + 64 * y == x);
  }
};

int main(int argc, const char * argv[])
{
  int t; std::cin >> t;

  for (int i = 0; i != t; ++i)
  {
    int x, y;

    std::cin >> x >> y;

    CheckValues<6>::check(x, y);

    std::cout << "\n";
  }
}
