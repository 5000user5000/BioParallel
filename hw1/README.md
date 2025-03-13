# HW1 Overview

This homework1 is divided into two parts: the **Matrix Class** implementation and the **Thread Pool** implementation. Each part has its own source and test files, and the entire homework1 is built using a single Makefile.

---

## Directory Structure

```
.
├── matrix.hpp        // Matrix class declarations and definitions (Part I)
├── matrix.cpp        // Matrix class implementation (Part I)
├── main.cpp          // Test program for the matrix functionality (Part I)
├── thread_pool.hpp   // Thread pool class declarations and definitions (Part II)
├── thread_pool.cpp   // Thread pool class implementation (Part II)
├── test.cpp          // Test program for the thread pool functionality (Part II)
└── Makefile          // Build and test instructions
```

---

## Homework1 Requirements

### Part I – Matrix Class

- **Matrix Class Design**  
  Define two matrix classes:
  - `Column_Major_Matrix<T>`: Stores data in a `vector<vector<T>>` named `all_column`.
  - `Row_Major_Matrix<T>`: Stores data in a `vector<vector<T>>` named `all_row`.

- **Constructors**  
  Provide constructors that accept dimensions (e.g., `Column_Major_Matrix<int> cc1(1000, 1000);`) and initialize all elements with random values of type `T`.

- **Access Methods**  
  Implement getter and setter functions to access rows or columns by index.

- **Copy and Move Operations**  
  Overload the copy constructor, copy assignment operator, move constructor, and move assignment operator so that the following code works correctly:
  ```cpp
  Column_Major_Matrix<int> cc1(1000, 1000);
  Row_Major_Matrix<int> rr1(1000, 1000);
  Column_Major_Matrix<int> cc2(cc1);
  Row_Major_Matrix<int> rr2 = rr1;
  Column_Major_Matrix<int> cc3 = std::move(cc2);
  Row_Major_Matrix<int> rr3 = std::move(rr2);
  ```

- **Matrix Multiplication**  
  Overload the multiplication operator (`operator*`) for:
  - `Row_Major_Matrix` multiplied by a `Column_Major_Matrix`, returning a `Row_Major_Matrix`.
  - `Column_Major_Matrix` multiplied by a `Row_Major_Matrix`, returning a `Column_Major_Matrix`.

- **Type Conversion Operators**  
  Implement implicit conversion operators (`operator Row_Major_Matrix()` and `operator Column_Major_Matrix()`) to allow conversion between the two matrix types. For example:
  ```cpp
  Column_Major_Matrix<int> cc(55, 1000);
  Row_Major_Matrix<int> rr(1000, 66);
  Row_Major_Matrix<int> result = cc * rr;
  ```

- **Multithreading Acceleration**  
  Overload the `%` operator to perform matrix multiplication using exactly 10 threads. Use `std::chrono` to display the speedup with and without multithreading.

> **Source Files:** `matrix.hpp`, `matrix.cpp`, `main.cpp`

---

### Part II – Thread Pool

- **Thread Pool Design**  
  Design a ThreadPool class with the following features:
  - Allows users to submit jobs (tasks) to the pool.
  - Supports any kind of callable objects as jobs.
  - Maintains a job queue (e.g., using `std::function` or `std::bind` or `std::packaged_task`) for unfinished tasks.
  - Starts with 5 threads that always wait for new jobs; each thread should record its total running time.
  - Threads are only terminated (joined) when the thread pool is destructed. Upon destruction, the pool prints the total running time and `std::thread::id` for each thread.
  - Uses condition variables and mutexes to notify threads when there are new tasks.

- **Additional Tasks**  
  - Implement a function `print_1` that generates a random integer and prints `'1'` if the number is odd or `'0'` otherwise. (Note: `std::cout` is a shared resource and must be properly synchronized.)
  - Implement a functor `print_2` that simply prints `"2"`. Use a condition variable to ensure that `print_2` executes only after all `print_1` tasks have been completed.
  - In the test program (`main()` in `test.cpp`), first submit 496 `print_1` tasks, followed by 4 `print_2` tasks.

> **Source Files:** `thread_pool.hpp`, `thread_pool.cpp`, `test.cpp`

---

## Build and Test Instructions

The homework1 uses a Makefile to manage compilation. You can compile and test the parts separately as follows:

- **To build the entire homework1 (both parts):**
  ```sh
  make
  ```

- **To compile only Part I (Matrix Class):**
  ```sh
  make main
  ```

- **To compile only Part II (Thread Pool):**
  ```sh
  make test
  ```

- **To run tests for Part I:**
  ```sh
  make check
  ```

- **To run tests for Part II:**
  ```sh
  make runtest
  ```

- **To clean up build files:**
  ```sh
  make clean
  ```

---

## Additional Notes

- **Compiler Options:**  
  Ensure you compile with C++17 and enable pthread support. For example:
  ```sh
  g++ -std=c++17 -pthread -O2 ...
  ```

- **Thread Safety:**  
  Proper mutexes and condition variables are used to synchronize shared resources such as `std::cout`, atomic variables, and condition checks.

- **Output Verification:**  
  The test results should include the execution times of the threads along with their corresponding `std::thread::id`.