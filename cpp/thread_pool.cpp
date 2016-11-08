#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <chrono>
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
      std::lock_guard lk(m_mutex);
      return m_queue.empty();
   }

   // called by writer threads
   void Push(ItemType item)
   {
      std::lock_guard<std::mutex> lk(m_mutex);
      m_queue.push(item);
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

class ITask
{
public:
   virtual ~ITask()
   {}

   // Template pattern
   void Execute()
   {
      DoExecute();
      m_promise.set_value(true);
   }

protected:
   // Synchronize w/ the result(blocking wait)
   void WaitResult()
   {
      m_promise.get_future().get();
   }

   virtual void DoExecute() = 0;

private:
   std::promise<bool> m_promise;
};

template<typename Type>
class AddTask : public ITask
{
public:
   AddTask(Type a, Type b): m_a(a), m_b(b)
   {}

   Type Get()
   {
      WaitResult();
      return m_res;
   }

private:
   void DoExecute() override
   {
      std::this_thread::sleep_for(std::chrono::seconds(3));
      m_res = m_a + m_b;
   }

   Type m_a;
   Type m_b;
   Type m_res;
};

template<typename Type>
class MultiplyTask : public ITask
{
public:
   MultiplyTask(Type a, Type b): m_a(a), m_b(b)
   {}

   Type Get()
   {
      WaitResult();
      return m_res;
   }
private:
   void DoExecute() override
   {
      std::this_thread::sleep_for(std::chrono::seconds(3));
      m_res = m_a * m_b;
   }

   Type m_a;
   Type m_b;
   Type m_res;
};

class ThreadPool
{
public:
   ThreadPool(size_t size = -1) : m_exit(false)
   {
      if (size <= 0) {
         // Create at least one thread
         size = std::max(std::thread::hardware_concurrency(), 1u);
      }
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

   void Submit(shared_ptr<ITask> task)
   {
      m_queue.Push(task);
   }
private:
   void Destroy()
   {
      m_exit = true;
      // Must follow m_exit! Think!
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
         shared_ptr<ITask> task;
         if (m_queue.Pop(task)) {
            task->Execute();
         }
      }
   }

   std::atomic_bool m_exit;
   ThreadSafeQueue<std::shared_ptr<ITask>> m_queue;
   std::vector<std::thread> m_threads;
};

void f()
{
   static int s_counter = 0;
   static std::mutex s_ioMutex;

   std::lock_guard<std::mutex> lk(s_ioMutex);
   cout << "[" << ++s_counter << "] Thread " << std::this_thread::get_id() << " is executing.." << endl;
}

int main()
{
   ThreadPool pool(6);
   std::shared_ptr<AddTask<int>> task1 = std::make_shared<AddTask<int>>(10, 20);
   std::shared_ptr<AddTask<double>> task2 = std::make_shared<AddTask<double>>(11.1, 22.2);
   std::shared_ptr<MultiplyTask<int>> task3 = std::make_shared<MultiplyTask<int>>(10, 20);
   std::shared_ptr<MultiplyTask<double>> task4 = std::make_shared<MultiplyTask<double>>(11.1, 22.2);
   pool.Submit(task1);
   pool.Submit(task2);
   pool.Submit(task3);
   pool.Submit(task4);

   cout << task1->Get() << endl;
   cout << task2->Get() << endl;
   cout << task3->Get() << endl;
   cout << task4->Get() << endl;

   cout << "[Caller]: received results from all threads.\n";

   getchar();
   return 0;
}