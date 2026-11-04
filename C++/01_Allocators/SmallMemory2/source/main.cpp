/**
 * Memory
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

#include "SmallMemory2.hpp"

int main(int argc, const char * argv[])
{
  SmallMemory2 pool(4);

  // Allocation of multiple blocks
  void* p1 = pool.Alloc();
  void* p2 = pool.Alloc();
  void* p3 = pool.Alloc();
  void* p4 = pool.Alloc();

  std::cout << "Allocated blocks: " << p1 << ", " << p2 << ", " << p3 << ", " << p4 << std::endl;

  // Attempt to allocate beyond the limit
  void* p5 = pool.Alloc();

  if (!p5)
  {
      std::cout << "No free blocks left!" << std::endl;
  }

  // Freeing up a couple of blocks
  pool.Free(p2);
  pool.Free(p4);

  // Allocate again - should get p4 and p2 back
  void* p6 = pool.Alloc();
  void* p7 = pool.Alloc();

  std::cout << "Reallocated blocks: " << p6 << ", " << p7 << std::endl;

  return 0;
}
