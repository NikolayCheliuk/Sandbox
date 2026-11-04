/**
 * SmallMemory1.cpp
 */

#include "SmallMemory1.hpp"

SmallMemory1::SmallMemory1(size_t capacity) :
  _capacity(capacity),
  _memory(capacity)
{
  for (size_t i = 0; i < (_capacity - 1); i++)
  {
    _memory[i]._next = &_memory[i + 1];
  }

  _memory[_capacity - 1]._next = nullptr;

  _free_blocks = &_memory[0];
}

void * SmallMemory1::Alloc()
{
    void * ptr = nullptr;
    MemoryBlock * block = nullptr;

    if (_free_blocks)
    {
        block = _free_blocks;
        _free_blocks = block->_next;
        ptr = static_cast<void *>(block->_memory);
    }

    return ptr;
}

void SmallMemory1::Free(void * ptr)
{
  MemoryBlock * block;

  if (!ptr)
  {
    // Handle Error
  }
  else
  {
    block = reinterpret_cast<MemoryBlock *>(reinterpret_cast<char *>(ptr) - offsetof(MemoryBlock, _memory));
    block->_next = _free_blocks;
    _free_blocks = block;
  }
}
