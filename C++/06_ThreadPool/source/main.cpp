/**
 * Thread Pool
 */

#include "ThreadPool.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
  ThreadPool pool(4);

  for (size_t i = 0; i < 10; i++)
  {
    pool.enqueue(
      [i]()
      {
        std::cout << "Task " << i << " executed by thread " << std::this_thread::get_id() << std::endl;
      }
    );
  }

  return 0;
}
