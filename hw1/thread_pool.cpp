#include "thread_pool.hpp"
#include <iostream>

ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> job;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->jobs.empty(); });
                    if (this->stop && this->jobs.empty()) return;
                    job = std::move(this->jobs.front());
                    this->jobs.pop();
                }
                job();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers)
        worker.join();
}

void ThreadPool::enqueue(std::function<void()> job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.emplace(job);
    }
    condition.notify_one();
}
