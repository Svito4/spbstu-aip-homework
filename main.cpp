#include <cstddef>
#include <iostream>
#include <limits>

namespace shcherbakov {
  int ** allocateMatrix(std::size_t rows, std::size_t cols);
  void freeMatrix(const int * const * matrix, std::size_t rows);
  void createMatrix(std::size_t rows, std::size_t cols, int * const * matrix);
  int ** concat_rows(const int * const * lhs,
      std::size_t rows1,
      std::size_t cols1,
      const int * const * rhs,
      std::size_t rows2,
      std::size_t cols2);
  void
  printMatrix(const int * const * matrix, std::size_t rows1, std::size_t cols1, std::size_t rows2, std::size_t cols2);
}

int ** shcherbakov::allocateMatrix(std::size_t rows, std::size_t cols)
{
  int ** matrix = nullptr;
  try {
    matrix = new int *[rows];
    for (std::size_t i = 0; i < rows; ++i) {
      matrix[i] = nullptr;
    }
    for (std::size_t i = 0; i < rows; ++i) {
      matrix[i] = new int[cols];
    }
  } catch (...) {
    if (matrix != nullptr) {
      for (std::size_t i = 0; i < rows; ++i) {
        delete[] matrix[i];
      }
      delete[] matrix;
    }
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

void shcherbakov::createMatrix(std::size_t rows, std::size_t cols, int * const * const matrix)
{
  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < cols; ++j) {
      std::cin >> matrix[i][j];
    }
  }
}

int ** shcherbakov::concat_rows(const int * const * const lhs,
    std::size_t rows1,
    std::size_t cols1,
    const int * const * const rhs,
    std::size_t rows2,
    std::size_t cols2)
{
  if (cols1 > (std::numeric_limits< std::size_t >::max() - cols2)) {
    return nullptr;
  }
  const std::size_t rows = (rows1 > rows2) ? rows1 : rows2;
  int ** result = nullptr;
  try {
    result = new int *[rows];
    for (std::size_t i = 0; i < rows; ++i) {
      result[i] = nullptr;
    }
    for (std::size_t i = 0; i < rows; ++i) {
      const std::size_t left_cols = (i < rows1) ? cols1 : 0;
      const std::size_t right_cols = (i < rows2) ? cols2 : 0;
      result[i] = new int[left_cols + right_cols];
      for (std::size_t j = 0; j < left_cols; ++j) {
        result[i][j] = lhs[i][j];
      }
      for (std::size_t j = 0; j < right_cols; ++j) {
        result[i][left_cols + j] = rhs[i][j];
      }
    }
  } catch (...) {
    freeMatrix(result, rows);
    return nullptr;
  }
  return result;
}

void shcherbakov::printMatrix(const int * const * const matrix,
    std::size_t rows1,
    std::size_t cols1,
    std::size_t rows2,
    std::size_t cols2)
{
  const std::size_t rows = (rows1 > rows2) ? rows1 : rows2;
  for (std::size_t i = 0; i < rows; ++i) {
    const std::size_t left_cols = (i < rows1) ? cols1 : 0;
    const std::size_t right_cols = (i < rows2) ? cols2 : 0;
    const std::size_t cols = left_cols + right_cols;
    for (std::size_t j = 0; j < cols; ++j) {
      std::cout << matrix[i][j];
      if (j + 1 < cols) {
        std::cout << " ";
      }
    }
    std::cout << "\n";
  }
}

int main()
{
  const int input_error = 1;
  const int memory_error = 2;
  int rows1 = 0;
  int cols1 = 0;
  if (!(std::cin >> rows1 >> cols1) || (rows1 <= 0) || (cols1 <= 0)) {
    return input_error;
  }
  int * const * const matrix1 = shcherbakov::allocateMatrix(rows1, cols1);
  if (matrix1 == nullptr) {
    return memory_error;
  }
  shcherbakov::createMatrix(rows1, cols1, matrix1);
  if (!std::cin) {
    shcherbakov::freeMatrix(matrix1, rows1);
    return input_error;
  }

  int rows2 = 0;
  int cols2 = 0;
  if (!(std::cin >> rows2 >> cols2) || (rows2 <= 0) || (cols2 <= 0)) {
    shcherbakov::freeMatrix(matrix1, rows1);
    return input_error;
  }
  int * const * const matrix2 = shcherbakov::allocateMatrix(rows2, cols2);
  if (matrix2 == nullptr) {
    shcherbakov::freeMatrix(matrix1, rows1);
    return memory_error;
  }
  shcherbakov::createMatrix(rows2, cols2, matrix2);
  if (!std::cin) {
    shcherbakov::freeMatrix(matrix2, rows2);
    shcherbakov::freeMatrix(matrix1, rows1);
    return input_error;
  }

  const int * const * const result = shcherbakov::concat_rows(matrix1, rows1, cols1, matrix2, rows2, cols2);
  if (result == nullptr) {
    shcherbakov::freeMatrix(matrix2, rows2);
    shcherbakov::freeMatrix(matrix1, rows1);
    return memory_error;
  }
  shcherbakov::printMatrix(result, rows1, cols1, rows2, cols2);
  const int result_rows = (rows1 > rows2) ? rows1 : rows2;
  shcherbakov::freeMatrix(result, result_rows);
  shcherbakov::freeMatrix(matrix2, rows2);
  shcherbakov::freeMatrix(matrix1, rows1);
  return 0;
}
