#include "thread_pool.hpp"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <atomic>

std::mutex cout_mutex;
std::condition_variable cv;
std::atomic<int> print1_count(0); //  print_1 task count 
std::mutex cv_mutex; // condition variable mutex to protect print1_count

// `print_1` function，print 1 or 0 randomly
void print_1() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 100);

    int num = dist(gen);
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << (num % 2 ? "1" : "0") << std::flush;
    }

    // decrease the print_1 task count
    if (print1_count.fetch_sub(1) == 1) { // if the last print_1 task
        cv.notify_all(); // notify all print_2 tasks
    }
}

// `print_2` functor
struct print_2 {
    void operator()() {
        std::unique_lock<std::mutex> lock(cv_mutex);
        cv.wait(lock, [] { return print1_count.load() == 0; }); // wait until all print_1 tasks are done

        std::lock_guard<std::mutex> lock_cout(cout_mutex);
        std::cout << "2" << std::flush;
    }
};

int main() {
    ThreadPool pool(5); // build a thread pool with 5 threads

    print1_count = 496; // set print_1 task count to 496

    // commit 496 `print_1` tasks
    for (int i = 0; i < 496; ++i) {
        pool.enqueue(print_1);
    }

    // commit 4 `print_2` tasks
    for (int i = 0; i < 4; ++i) {
        pool.enqueue(print_2());
    }

    // wait for all tasks to complete
    std::this_thread::sleep_for(std::chrono::seconds(3));

    return 0;
}