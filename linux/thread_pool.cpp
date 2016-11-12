#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <condition_variable>
using namespace std;

template<typename ItemType>
class ThreadSafeQueue
{
public:
   ThreadSafeQueue() = default;
   ~ThreadSafeQueue()
   {
      Invalidate();
   }
   ThreadSafeQueue(const ThreadSafeQueue&) = delete;
   ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

   bool Empty() const
   {
      std::lock_guard<std::mutex> lk(m_mutex);
      return m_queue.empty();
   }

   // called by writer threads
   void Push(const ItemType &func)
   {
      std::lock_guard<std::mutex> lk(m_mutex);
      m_queue.push(func);
      m_cond.notify_one();
   }

   // called by reader threads
   bool Pop(ItemType &item)
   {
      std::unique_lock<std::mutex> lk(m_mutex);
      m_cond.wait(lk, [this]() { return !this->m_queue.empty() || !m_valid; });

      // Cooperate w/ notify_all(). Important for application exit.
      if (!m_valid) {
         return false;
      }
      item = m_queue.front();
      m_queue.pop();
      return true;
   }

   // Provides a way to notify all blocked threads when
   // this queue is to be destructed
   void Invalidate()
   {
      std::lock_guard<std::mutex> lk(m_mutex);
      m_valid = false;
      m_cond.notify_all();
   }

   bool IsValid() const
   {
      std::lock_guard<std::mutex> lk(m_mutex);
      return m_valid;
   }

private:
   std::atomic_bool m_valid{ true };
   mutable std::mutex m_mutex;
   std::condition_variable m_cond;
   std::queue<ItemType> m_queue;
};

class ThreadPool
{
public:
   ThreadPool(size_t size) : m_exit(false)
   {
      try {
         for (size_t i = 0; i < size; ++i) {
            // &ThreadPool::worker is a must!
            m_threads.push_back(std::thread(&ThreadPool::worker, this));
         }
      }
      catch (const std::bad_alloc&) {
         Destroy();
         throw;
      }
   }

   ~ThreadPool()
   {
      Destroy();
   }

   ThreadPool(const ThreadPool&) = delete;
   ThreadPool& operator=(const ThreadPool&) = delete;

   template<typename Function>
   void Submit(Function item)
   {
      m_queue.Push(FunctionType(item));
   }

private:
   void Destroy()
   {
      m_exit = true;
      // Must follow m_exit!
      // This is because a fast worker would wake, pass the test of m_exit
      // and wrap around to enter Pop() to block again.
      m_queue.Invalidate();

      for (auto &i : m_threads) {
         if (i.joinable()) {
            i.join();
         }
      }
   }

   void worker()
   {
      while (!m_exit) {
         FunctionType item;
         if (m_queue.Pop(item)) {
            item();
         }
      }
   }

   using FunctionType = std::function<void()>;
   std::atomic_bool m_exit;
   ThreadSafeQueue<FunctionType> m_queue;
   std::vector<std::thread> m_threads;
   // Used to fetch results
   std::vector<std::promise> m_promises;
};

void calSum(int begin, int end, std::promise<int>& p)
{
   static int s_counter = 0;
   static std::mutex s_ioMutex;

   int res = 0;
   for (int i = begin; i <= end; ++i) {
       res += i;
   }

   std::lock_guard<std::mutex> lk(s_ioMutex);
   cout << "[" << ++s_counter << "] Thread " << std::this_thread::get_id()
        << ": sum[" << begin << ", " << end << "]" << endl;

   p.set_value(res);
}

int main()
{
   ThreadPool pool(6);
   for (int i = 0; i < 100; ++i) {
      pool.Submit(calSum);
   }

   getchar();
   return 0;
}
