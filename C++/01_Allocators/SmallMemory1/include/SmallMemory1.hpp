/**
 * SmallMemory1.hpp
 */

#pragma once

#include <list>
#include <memory>
#include <vector>

#define BLOCK_SIZE 32

struct MemoryBlock
{
public:
  MemoryBlock * _next;
  char _memory[BLOCK_SIZE];
};

class SmallMemory1
{
public:
  SmallMemory1(size_t capacity);

  void * Alloc();
  void Free(void * ptr);

private:
  size_t _capacity;
  std::vector<MemoryBlock> _memory;
  MemoryBlock * _free_blocks;
};
