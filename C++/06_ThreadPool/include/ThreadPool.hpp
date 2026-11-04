/**
 * ThreadPool.hpp
 */

#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
  public:
    explicit ThreadPool(size_t threads_number);
    ~ThreadPool();

    void enqueue(std::function<void()> task);

  private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;

    std::mutex _queue_mutex;
    std::condition_variable _queue_conditional_variable;
    std::atomic<bool> _stop;
};
