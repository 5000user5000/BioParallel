#include "matrix.hpp"
#include "thread_pool.hpp"
#include <iostream>
#include <chrono>

void test_matrix_operations() {
    std::cout << "Testing matrix multiplication...\n";

    Row_Major_Matrix<int> rm(100, 100);
    Column_Major_Matrix<int> cm(100, 100);

    auto start = std::chrono::high_resolution_clock::now();
    Row_Major_Matrix<int> result = rm * cm;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Single-threaded multiplication took "
              << std::chrono::duration<double>(end - start).count() << " seconds.\n";

    start = std::chrono::high_resolution_clock::now();
    Row_Major_Matrix<int> result_parallel = rm % cm;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Multi-threaded multiplication took "
              << std::chrono::duration<double>(end - start).count() << " seconds.\n";
}

void test_thread_pool() {
    std::cout << "Testing thread pool...\n";
    
    ThreadPool pool(5);

    for (int i = 0; i < 10; ++i) {
        pool.enqueue([i] {
            std::cout << "Executing Task " << i << " on thread " << std::this_thread::get_id() << "\n";
        });
    }
}

int main() {
    test_matrix_operations();
    test_thread_pool();
    return 0;
}
