#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>

class ThreadPool {
public:
    ThreadPool(size_t threads = 5);
    ~ThreadPool();
    void enqueue(std::function<void()> job);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

#endif
