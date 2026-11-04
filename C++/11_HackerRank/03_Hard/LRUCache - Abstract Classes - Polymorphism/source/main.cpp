/**
 * LRUCache
 *
  Sample Input:
  3 1
  set 1 2
  get 1
  get 2

  Sample Output:
  2
  -1
 *
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>


struct LRUCacheNode
{
  public:
    LRUCacheNode(int key, int Value) :
      key_(key),
      value_(Value),
      next_(nullptr),
      prev_(nullptr)
    {

    }

    int key_;
    int value_;
    LRUCacheNode * next_;
    LRUCacheNode * prev_;
};

class LRUCache
{
  public:

    LRUCache(int capacity) :
      capacity_(capacity),
      head_(nullptr),
      tail_(nullptr)
    {
      cache_map_.reserve(capacity_);
    }

    LRUCache(const LRUCache &) = delete;

    ~LRUCache()
    {
      while (head_)
      {
        LRUCacheNode * old_head = head_;
        head_ = head_->next_;
        delete old_head;
      }

      head_ = nullptr;
      tail_ = nullptr;
    }

    LRUCache& operator = (const LRUCache &) = delete;

    int get(int key)
    {
      if (capacity_ == 0)
      {
        return -1;
      }

      auto it = cache_map_.find(key);

      if (it != cache_map_.end())
      {
        LRUCacheNode * node = it->second;

        if (node == head_)
        {
          // do nothing
        }
        else
        {
          move_to_head(node);
        }

        return node->value_;
      }
      else
      {
        return -1;
      }
    }

    void set(int key, int Value)
    {
      if (capacity_ == 0)
      {
        return;
      }

      auto it = cache_map_.find(key);

      if (it != cache_map_.end())
      {
        LRUCacheNode * node = it->second;

        node->value_ = Value;

        if (node == head_)
        {
          // do nothing
        }
        else
        {
          move_to_head(node);
        }
      }
      else
      {
        if (cache_map_.size() == capacity_)
        {
          if (head_ == tail_)
          {
            cache_map_.erase(tail_->key_);

            delete head_;
            head_ = nullptr;
            tail_ = nullptr;
          }
          else
          {
            cache_map_.erase(tail_->key_);

            LRUCacheNode * old_tail = tail_;
            tail_ = tail_->prev_;
            tail_->next_ = nullptr;
            delete old_tail;
          }
        }

        LRUCacheNode * node = new LRUCacheNode(key, Value);

        node->prev_ = nullptr;
        node->next_ = head_;

        if (head_)
        {
          head_->prev_ = node;
        }

        head_ = node;

        if (!tail_)
        {
          tail_ = node;
        }

        cache_map_[key] = node;
      }
    }

  private:

    void move_to_head(LRUCacheNode * node)
    {
      if (!node || node == head_)
      {
        return;
      }

      if (node == tail_)
      {
        tail_ = node->prev_;
      }

      if (node->prev_)
      {
        node->prev_->next_ = node->next_;
      }

      if (node->next_)
      {
        node->next_->prev_ = node->prev_;
      }

      node->next_ = head_;
      node->prev_ = nullptr;
      head_->prev_ = node;
      head_ = node;
    }

  private:

    std::unordered_map<int, LRUCacheNode *> cache_map_;
    LRUCacheNode * head_;
    LRUCacheNode * tail_;
    size_t capacity_;
};

int main(int argc, const char * argv[])
{
  int n;
  int capacity;
  int i;

  std::cin >> n >> capacity;

  LRUCache l(capacity);

  for (i = 0; i < n; i++)
  {
    std::string command;
    std::cin >> command;

    if(command == "get")
    {
      int key;
      std::cin >> key;
      std::cout << l.get(key) << std::endl;
    }
    else if(command == "set")
    {
      int key, Value;
      std::cin >> key >> Value;
      l.set(key,Value);
    }
  }

  return 0;
}
