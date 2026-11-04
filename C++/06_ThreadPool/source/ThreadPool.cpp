/**
 * ThreadPool.cpp
 */

#include "ThreadPool.hpp"

#include <iostream>

ThreadPool::ThreadPool(size_t threads_number) :
  _stop(false)
{
  for (size_t i = 0; i < threads_number; i++)
  {
    _workers.emplace_back(
      [this]
      {
        while (true)
        {
          std::function<void()> task;

          {
            std::unique_lock<std::mutex> lock(this->_queue_mutex);

            this->_queue_conditional_variable.wait(
              lock,
              [this]
              {
                return this->_stop || !this->_tasks.empty();
              }
            );

            if (this->_stop == true && this->_tasks.empty())
            {
              return;
            }

            task = std::move(this->_tasks.front());
            this->_tasks.pop();
          }

          task();
        }
      }
    );
  }
}

ThreadPool::~ThreadPool()
{
  {
    std::unique_lock<std::mutex> lock(this->_queue_mutex);
    _stop = true;
  }

  _queue_conditional_variable.notify_all();

  for (std::thread & worker : _workers)
  {
    worker.join();
  }
}

void ThreadPool::enqueue(std::function<void()> task)
{
  {
    std::unique_lock<std::mutex> lock(this->_queue_mutex);

    if (_stop == true)
    {
      return;
    }
    else
    {
      _tasks.push(std::move(task));
    }
  }

  _queue_conditional_variable.notify_one();
}
