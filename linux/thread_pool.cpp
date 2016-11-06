#include <iostream>
#include <queue>
#include <mutex>

/*
 * Implements a multithreaded safe circular FIFO.
 */
template <class T>
class TaskQueue
{
public:
    TaskQueue():
    {
    }

    void Enqueue(T task)
    {
        std::lock_guard<std::mutex> lck(m_mutex);
        m_queue.push(task);
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
};

int main()
{
    return 0;
}

