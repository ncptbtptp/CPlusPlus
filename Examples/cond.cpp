#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
using namespace std;

std::mutex g_mtx;
// Unfortunately this 2nd mutex is used to protect concurrent
// access to std::cout to avoid garbage outputs!
std::mutex g_mtxCout;
std::condition_variable g_cond;

// Shared data among threads
std::queue<int> g_workingQueue;

void workerthread(int id)
{
    // We have to use unique_lock here for its flexibility to unlock
    // at will
    std::unique_lock<mutex> lk(g_mtx);
    g_cond.wait(lk, []{ return !g_workingQueue.empty(); });

    // When wait exits, there can't be spurious wakeup, good!
    int next_data = g_workingQueue.front();

    // It's our task
    if (next_data == id) {
        g_workingQueue.pop();
    }
    lk.unlock();

    // Process data
    std::lock_guard<std::mutex> lkCout(g_mtxCout);
    cout << "Item " << id << " is now being processed.\n";
}

int main()
{
    size_t kTaskNum = 10;
    std::thread ths[kTaskNum];

    // Spawn threads
    for (size_t i = 0; i < kTaskNum; ++i) {
        ths[i] = std::thread(workerthread, i);
    }


    // Feed new threads
    for (size_t i = 0; i < kTaskNum; ++i) {
        lock_guard<std::mutex> lk(g_mtx);
        g_workingQueue.push(i);
        // Notify all worker threads
        g_cond.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }


    for (auto &th : ths) {
        th.join();
    }

    return 0;
}
