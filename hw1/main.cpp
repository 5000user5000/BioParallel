#include "matrix.hpp"
#include <iostream>
#include <vector>
#include <cassert>
#include <numeric>  // std::iota

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

void test_matrix_operations(int RM_rows = 10, int RM_cols = 10, int CM_rows = 10, int CM_cols = 10) {
    std::cout << "===== Testing Matrix Operations =====" << std::endl;

    // Create Row Major and Column Major matrices
    Row_Major_Matrix<int> rowMatrix(RM_rows, RM_cols);
    Column_Major_Matrix<int> colMatrix(CM_rows, CM_cols);

    std::cout << "\n=== Row Major Matrix (Original) ===" << std::endl;
    // rowMatrix.print();

    std::cout << "\n=== Column Major Matrix (Original) ===" << std::endl;
    // colMatrix.print();

    // Test copy constructor
    Row_Major_Matrix<int> rowCopy(rowMatrix);
    Column_Major_Matrix<int> colCopy(colMatrix);
    assert(areRowMatricesEqual(rowMatrix, rowCopy) && "Row Major Matrix copy constructor failed!");
    assert(areColMatricesEqual(colMatrix, colCopy) && "Column Major Matrix copy constructor failed!");
    std::cout << "\n✅ Copy constructor test passed!" << std::endl;

    // Test assignment operator
    Row_Major_Matrix<int> rowAssigned = rowMatrix;
    Column_Major_Matrix<int> colAssigned = colMatrix;
    assert(areRowMatricesEqual(rowMatrix, rowAssigned) && "Row Major Matrix assignment operator failed!");
    assert(areColMatricesEqual(colMatrix, colAssigned) && "Column Major Matrix assignment operator failed!");
    std::cout << "✅ Assignment operator test passed!" << std::endl;

    // Test move assignment1
    Row_Major_Matrix<int> rowMoved(std::move(rowCopy));
    Column_Major_Matrix<int> colMoved(std::move(colCopy));
    assert(areRowMatricesEqual(rowMatrix, rowMoved) && "Row Major Matrix move assignment failed!");
    assert(areColMatricesEqual(colMatrix, colMoved) && "Column Major Matrix move assignment failed!");
    assert(rowCopy.all_row.empty() && "Row Major Matrix copy should be empty after move assignment");
    assert(colCopy.all_column.empty() && "Col Major Matrix copy should be empty after move assignment");
    std::cout << "✅ Move assignment test1 passed!" << std::endl;

    // Test move assignment2
    Row_Major_Matrix<int> rowMoveAssigned = std::move(rowAssigned);
    Column_Major_Matrix<int> colMoveAssigned = std::move(colAssigned);
    assert(areRowMatricesEqual(rowMatrix, rowMoveAssigned));
    assert(areColMatricesEqual(colMatrix, colMoveAssigned));
    assert(rowAssigned.all_row.empty() && "rowAssigned should be empty");
    assert(colAssigned.all_column.empty() && "colAssigned should be empty");
    std::cout << "✅ Move assignment test2 passed!" << std::endl;

    // Test getter / setter for Row Major Matrix
    std::vector<int> newRow(RM_cols);
    std::iota(newRow.begin(), newRow.end(), 1);
    rowMatrix.setRow(0, newRow);
    assert(rowMatrix.getRow(0) == newRow && "Row Major Matrix getter / setter failed!");
    std::cout << "\nRow Major Matrix - First Row After setRow(1, ...): " << std::endl;
    // for (int val : rowMatrix.getRow(0)) {
    //     std::cout << val << " ";
    // }
    std::cout << "\n✅ Getter / Setter test for Row Major Matrix passed!" << std::endl;

    // Test getter / setter for Column Major Matrix
    std::vector<int> newCol(CM_rows);
    std::iota(newCol.begin(), newCol.end(), 1);
    colMatrix.setColumn(0, newCol);
    assert(colMatrix.getColumn(0) == newCol && "Column Major Matrix getter / setter failed!");
    std::cout << "\nColumn Major Matrix - First Column After setColumn(1, ...): " << std::endl;
    // for (int val : colMatrix.getColumn(0)) {
    //     std::cout << val << " ";
    // }
    std::cout << "\n✅ Getter / Setter test for Column Major Matrix passed!" << std::endl;

    // Single-threaded matrix multiplication: Row Major * Column Major
    std::cout << "\n=== Row Major * Column Major (Single-threaded) ===" << std::endl;
    Row_Major_Matrix<int> result1 = rowMatrix * colMatrix;
    // result1.print();

    // Single-threaded matrix multiplication: Column Major * Row Major
    std::cout << "\n=== Column Major * Row Major (Single-threaded) ===" << std::endl;
    Column_Major_Matrix<int> result2 = colMatrix * rowMatrix;
    // result2.print();

    // Multi-threaded matrix multiplication: Row Major * Column Major
    std::cout << "\n=== Row Major * Column Major (Multi-threaded) ===" << std::endl;
    Row_Major_Matrix<int> result3 = rowMatrix % colMatrix;
    // result3.print();

    // Multi-threaded matrix multiplication: Column Major * Row Major
    std::cout << "\n=== Column Major * Row Major (Multi-threaded) ===" << std::endl;
    Column_Major_Matrix<int> result4 = colMatrix % rowMatrix;
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
    int RM_rows = 100, RM_cols = 100, CM_rows = 100, CM_cols = 100;
    test_matrix_operations(RM_rows, RM_cols, CM_rows, CM_cols);
    return 0;
}