#include <cstddef>
#include <iostream>
#include <limits>
#include <new>

namespace shcherbakov {
  int ** allocateMatrix(std::size_t rows, std::size_t columns);
  void freeMatrix(const int * const * matrix, std::size_t rows);
  bool readMatrix(int * const * matrix, std::size_t rows, std::size_t columns);
  int ** concat_rows(const int * const * lhs,
      std::size_t n1,
      std::size_t m1,
      const int * const * rhs,
      std::size_t n2,
      std::size_t m2);
  void printRows(const int * const * matrix, std::size_t n1, std::size_t m1, std::size_t n2, std::size_t m2);
}

int ** shcherbakov::allocateMatrix(std::size_t rows, std::size_t columns)
{
  int ** matrix = nullptr;
  try {
    matrix = new int *[rows]();
    for (std::size_t i = 0; i < rows; ++i) {
      matrix[i] = new int[columns];
    }
  } catch (const std::bad_alloc &) {
    freeMatrix(matrix, rows);
    return nullptr;
  }
  return matrix;
}

void shcherbakov::freeMatrix(const int * const * const matrix, std::size_t rows)
{
  if (matrix == nullptr) {
    return;
  }
  for (std::size_t i = 0; i < rows; ++i) {
    delete[] matrix[i];
  }
  delete[] matrix;
}

bool shcherbakov::readMatrix(int * const * const matrix, std::size_t rows, std::size_t columns)
{
  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < columns; ++j) {
      if (!(std::cin >> matrix[i][j])) {
        return false;
      }
    }
  }
  return true;
}

int ** shcherbakov::concat_rows(const int * const * const lhs,
    std::size_t n1,
    std::size_t m1,
    const int * const * const rhs,
    std::size_t n2,
    std::size_t m2)
{
  if (m1 > (std::numeric_limits< std::size_t >::max() - m2)) {
    return nullptr;
  }
  const std::size_t rows = (n1 > n2) ? n1 : n2;
  int ** result = nullptr;
  try {
    result = new int *[rows]();
    for (std::size_t i = 0; i < rows; ++i) {
      const std::size_t left_columns = (i < n1) ? m1 : 0;
      const std::size_t right_columns = (i < n2) ? m2 : 0;
      result[i] = new int[left_columns + right_columns];
      for (std::size_t j = 0; j < left_columns; ++j) {
        result[i][j] = lhs[i][j];
      }
      for (std::size_t j = 0; j < right_columns; ++j) {
        result[i][left_columns + j] = rhs[i][j];
      }
    }
  } catch (const std::bad_alloc &) {
    freeMatrix(result, rows);
    return nullptr;
  }
  return result;
}

void shcherbakov::printRows(const int * const * const matrix,
    std::size_t n1,
    std::size_t m1,
    std::size_t n2,
    std::size_t m2)
{
  const std::size_t rows = (n1 > n2) ? n1 : n2;
  for (std::size_t i = 0; i < rows; ++i) {
    const std::size_t left_columns = (i < n1) ? m1 : 0;
    const std::size_t right_columns = (i < n2) ? m2 : 0;
    const std::size_t columns = left_columns + right_columns;
    for (std::size_t j = 0; j < columns; ++j) {
      if (j != 0) {
        std::cout << ' ';
      }
      std::cout << matrix[i][j];
    }
    std::cout << '\n';
  }
}

int main()
{
  const int input_error = 1;
  const int memory_error = 2;
  long long rows = 0;
  long long columns = 0;
  if (!(std::cin >> rows >> columns) || (rows <= 0) || (columns <= 0)) {
    std::cerr << "Invalid first matrix size\n";
    return input_error;
  }
  const std::size_t n1 = rows;
  const std::size_t m1 = columns;
  int * const * const lhs = shcherbakov::allocateMatrix(n1, m1);
  if (lhs == nullptr) {
    std::cerr << "Not enough memory\n";
    return memory_error;
  }
  if (!shcherbakov::readMatrix(lhs, n1, m1)) {
    shcherbakov::freeMatrix(lhs, n1);
    std::cerr << "Invalid first matrix elements\n";
    return input_error;
  }

  if (!(std::cin >> rows >> columns) || (rows <= 0) || (columns <= 0)) {
    shcherbakov::freeMatrix(lhs, n1);
    std::cerr << "Invalid second matrix size\n";
    return input_error;
  }
  const std::size_t n2 = rows;
  const std::size_t m2 = columns;
  int * const * const rhs = shcherbakov::allocateMatrix(n2, m2);
  if (rhs == nullptr) {
    shcherbakov::freeMatrix(lhs, n1);
    std::cerr << "Not enough memory\n";
    return memory_error;
  }
  if (!shcherbakov::readMatrix(rhs, n2, m2)) {
    shcherbakov::freeMatrix(rhs, n2);
    shcherbakov::freeMatrix(lhs, n1);
    std::cerr << "Invalid second matrix elements\n";
    return input_error;
  }

  const int * const * const result = shcherbakov::concat_rows(lhs, n1, m1, rhs, n2, m2);
  if (result == nullptr) {
    shcherbakov::freeMatrix(rhs, n2);
    shcherbakov::freeMatrix(lhs, n1);
    std::cerr << "Not enough memory\n";
    return memory_error;
  }
  shcherbakov::printRows(result, n1, m1, n2, m2);
  const std::size_t result_rows = (n1 > n2) ? n1 : n2;
  shcherbakov::freeMatrix(result, result_rows);
  shcherbakov::freeMatrix(rhs, n2);
  shcherbakov::freeMatrix(lhs, n1);
  return 0;
}
