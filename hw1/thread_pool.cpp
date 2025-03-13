#include "thread_pool.hpp"

ThreadPool::ThreadPool(size_t threads) : stop(false), 
    thread_run_times(threads),
    worker_ids(threads) // Initialize the worker_ids size with the number of threads
{
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this, i] {
            
            // Store the thread id of each worker
            {
                // use ids_mutex to protect the worker_ids <or it will stack smashing>
                std::lock_guard<std::mutex> lock(this->ids_mutex);
                worker_ids[i] = std::this_thread::get_id();
            }
            auto start_time = std::chrono::high_resolution_clock::now();
            
            while (true) {
                std::function<void()> job;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->jobs.empty(); });

                    if (this->stop && this->jobs.empty()) {
                        auto end_time = std::chrono::high_resolution_clock::now();
                        thread_run_times[i] = end_time - start_time;
                        return;
                    }

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
    
    for (size_t i = 0; i < workers.size(); ++i) {
        workers[i].join();
        std::cout << "\nThread " << worker_ids[i]
                  << " executed for " << thread_run_times[i].count() << " seconds.\n";
    }
}

void ThreadPool::enqueue(std::function<void()> job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.emplace(job);
    }
    condition.notify_one();
}