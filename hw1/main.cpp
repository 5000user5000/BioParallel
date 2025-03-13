#include "matrix.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>

// Auxiliary function: compare two Row_Major_Matrix for equality
template<typename T>
bool areRowMatricesEqual(const Row_Major_Matrix<T>& m1, const Row_Major_Matrix<T>& m2) {
    if (m1.all_row.size() != m2.all_row.size())
        return false;
    for (size_t i = 0; i < m1.all_row.size(); ++i) {
        if (m1.all_row[i].size() != m2.all_row[i].size())
            return false;
        for (size_t j = 0; j < m1.all_row[i].size(); ++j) {
            if (m1.all_row[i][j] != m2.all_row[i][j])
                return false;
        }
    }
    return true;
}

// Auxiliary function: compare two Column_Major_Matrix for equality
template<typename T>
bool areColMatricesEqual(const Column_Major_Matrix<T>& m1, const Column_Major_Matrix<T>& m2) {
    if (m1.all_column.size() != m2.all_column.size())
        return false;
    for (size_t i = 0; i < m1.all_column.size(); ++i) {
        if (m1.all_column[i].size() != m2.all_column[i].size())
            return false;
        for (size_t j = 0; j < m1.all_column[i].size(); ++j) {
            if (m1.all_column[i][j] != m2.all_column[i][j])
                return false;
        }
    }
    return true;
}

void test_matrix_operations() {
    std::cout << "===== Testing Matrix Operations =====" << std::endl;

    // Create 10x10 Row Major and Column Major matrices
    Row_Major_Matrix<int> rowMatrix(10, 10);
    Column_Major_Matrix<int> colMatrix(10, 10);

    std::cout << "\n=== Row Major Matrix (Original) ===" << std::endl;
    rowMatrix.print();

    std::cout << "\n=== Column Major Matrix (Original) ===" << std::endl;
    colMatrix.print();

    // Test copy constructor
    Row_Major_Matrix<int> rowCopy(rowMatrix);
    Column_Major_Matrix<int> colCopy(colMatrix);
    std::cout << "\n✅ Copy constructor test passed!" << std::endl;

    // Test assignment operator
    Row_Major_Matrix<int> rowAssigned = rowMatrix;
    Column_Major_Matrix<int> colAssigned = colMatrix;
    std::cout << "✅ Assignment operator test passed!" << std::endl;

    // Test move assignment
    Row_Major_Matrix<int> rowMoved(std::move(rowCopy));
    Column_Major_Matrix<int> colMoved(std::move(colCopy));
    std::cout << "✅ Move assignment test passed!" << std::endl;

    // Test move assignment
    Row_Major_Matrix<int> rowMoveAssigned = std::move(rowAssigned);
    Column_Major_Matrix<int> colMoveAssigned = std::move(colAssigned);
    std::cout << "✅ Move assignment test passed!" << std::endl;

    // Test getter / setter
    std::vector<int> newRow = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    rowMatrix.setRow(0, newRow);
    std::cout << "\nRow Major Matrix - First Row After setRow(0, ...): " << std::endl;
    for (int val : rowMatrix.getRow(0)) {
        std::cout << val << " ";
    }
    std::cout << "\n✅ Getter / Setter test passed!" << std::endl;

    // Single-threaded matrix multiplication: Row Major * Column Major
    std::cout << "\n=== Row Major * Column Major (Single-threaded) ===" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    Row_Major_Matrix<int> result1 = rowMatrix * colMatrix;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Single-threaded multiplication time:  " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";
    // result1.print();

    // Single-threaded matrix multiplication: Column Major * Row Major
    std::cout << "\n=== Column Major * Row Major (Single-threaded) ===" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    Column_Major_Matrix<int> result2 = colMatrix * rowMatrix;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Single-threaded multiplication time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";
    // result2.print();

    // Multi-threaded matrix multiplication: Row Major * Column Major
    std::cout << "\n=== Row Major * Column Major (Multi-threaded) ===" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    Row_Major_Matrix<int> result3 = rowMatrix % colMatrix;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Multi-threaded multiplication time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";
    // result3.print();

    // Multi-threaded matrix multiplication: Column Major * Row Major
    std::cout << "\n=== Column Major * Row Major (Multi-threaded) ===" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    Column_Major_Matrix<int> result4 = colMatrix % rowMatrix;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Multi-threaded multiplication time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";
    // result4.print();

    // Verify that single-threaded and multi-threaded results are consistent
    if (areRowMatricesEqual(result1, result3))
    std::cout << "\n✅ Row Major multiplication: Single-threaded and multi-threaded results match!" << std::endl;
    else
        std::cout << "\n❌ Row Major multiplication: Single-threaded and multi-threaded results do not match!" << std::endl;

    if (areColMatricesEqual(result2, result4))
        std::cout << "✅ Column Major multiplication: Single-threaded and multi-threaded results match!" << std::endl;
    else
        std::cout << "❌ Column Major multiplication: Single-threaded and multi-threaded results do not match!" << std::endl;

    // Test type conversion: Row Major -> Column Major
    std::cout << "\n=== Type Conversion: Row Major -> Column Major ===" << std::endl;
    Column_Major_Matrix<int> convertedCol = rowMatrix;
    // convertedCol.print();

    // Test type conversion: Column Major -> Row Major
    std::cout << "\n=== Type Conversion: Column Major -> Row Major ===" << std::endl;
    Row_Major_Matrix<int> convertedRow = colMatrix;
    // convertedRow.print();

    std::cout << "\n✅ All tests completed!" << std::endl;
}

int main() {
    test_matrix_operations();
    return 0;
}