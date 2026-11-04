/**
 * SlidingWindowMaximum
 *
   Sample Input:

   2
   5 2
   3 4 6 3 4
   7 4
   3 4 5 8 1 4 10

   Sample Output:

   4 6 6 4
   8 8 8 10
 *
 */

#include <iostream>
#include <deque>
#include <vector>

void printKMax(const std::vector<int> & arr, int n, int k)
{
  std::deque<int> d;

  for (size_t i = 0; i < k; i++)
  {
    while (!d.empty() && arr[d.back()] < arr[i])
    {
      d.pop_back();
    }

    d.push_back(i);
  }

  std::cout << arr[d.front()] << " ";

  for (size_t i = k; i < n; i++)
  {
    if (!d.empty() && d.front() == i - k)
    {
      d.pop_front();
    }

    while (!d.empty() && arr[d.back()] < arr[i])
    {
      d.pop_back();
    }

    d.push_back(i);

    std::cout << arr[d.front()] << " ";
  }

  std::cout << std::endl;
}

int main(int argc, const char * argv[])
{
	int t;
	std::cin >> t;

  while(t > 0)
  {
		int n;
    int k;

   	std::cin >> n >> k;

    int i;
    std::vector<int> arr;
    arr.resize(n);

    for(i = 0; i < n; i++)
    {
      std::cin >> arr[i];
    }

    printKMax(arr, n, k);
    t--;
  }

  return 0;
}
