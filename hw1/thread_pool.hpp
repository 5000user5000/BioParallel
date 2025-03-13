#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <chrono>
#include <iostream>

class ThreadPool {
public:
    ThreadPool(size_t threads = 5);
    ~ThreadPool();
    
    // use for commit a job to the thread pool
    void enqueue(std::function<void()> job);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;
    
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

    // store the run time of each thread
    std::vector<std::chrono::duration<double>> thread_run_times;

    // store the thread id of each worker
    std::vector<std::thread::id> worker_ids;

    // mutex to protect the worker_ids
    std::mutex ids_mutex;
};

#endif