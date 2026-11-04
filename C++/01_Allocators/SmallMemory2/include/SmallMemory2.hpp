/**
 * SmallMemory2.hpp
 */

#pragma once

#include <list>
#include <vector>

#define BLOCK_SIZE 32

class SmallMemory2
{
public:
  SmallMemory2(size_t capacity);

  void * Alloc();
  void Free(void * ptr);

private:
  size_t _capacity;
  std::vector<char> _memory;
  std::list<void *> _free_blocks;
};
