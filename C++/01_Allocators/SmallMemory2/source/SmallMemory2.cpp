/**
 * SmallMemory2.cpp
 */

#include "SmallMemory2.hpp"

SmallMemory2::SmallMemory2(size_t capacity) :
  _capacity(capacity),
  _memory(capacity * BLOCK_SIZE) // allocate one big buffer
{
  // Initialize free list with pointers to each block
  for (size_t i = 0; i < _capacity; i++)
  {
    _free_blocks.push_back(&_memory[i * BLOCK_SIZE]);
  }
}

void * SmallMemory2::Alloc()
{
    void * ptr = nullptr;

    // If there are free blocks, take the first one
    if (!_free_blocks.empty())
    {
        ptr = _free_blocks.front();
        _free_blocks.pop_front();
    }

    // Return either a valid block or nullptr if none available
    return ptr;
}

void SmallMemory2::Free(void * ptr)
{
  // Safety check: ignore null pointers
  if (!ptr)
  {
    // Handle Error: freeing nullptr
  }
  else
  {
    // Put the block back at the front of the free list
    _free_blocks.push_front(ptr);
  }
}
