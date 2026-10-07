#include <iostream>

int** allocate_matrix(int rows, int cols)
{
  int** matrix = nullptr;
  try {
    matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
      matrix[i] = nullptr;
    }
    for (int i = 0; i < rows; ++i) {
      matrix[i] = new int[cols];
    }
  } catch (...) {
    if (matrix != nullptr) {
      for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
      }
      delete[] matrix;
    }
    return nullptr;
  }
  return matrix;
}

void free_matrix(int** matrix, int rows)
{
  if (matrix == nullptr) {
    return;
  }
  for (int i = 0; i < rows; ++i) {
    delete[] matrix[i];
  }
  delete[] matrix;
}

int** transport_matrix(int** matrix, int rows, int cols)
{
  int** new_matrix_transport = allocate_matrix(cols, rows);
  if (new_matrix_transport == nullptr) {
    return nullptr;
  }

  for (int i = 0; i < cols; ++i) {
    for (int j = 0; j < rows; ++j) {
      new_matrix_transport[i][j] = matrix[j][i];
    }
  }
  return new_matrix_transport;
}

void print_matrix(int** matrix, int rows, int cols)
{
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      std::cout << matrix[i][j];
      if (j + 1 < cols) {
        std::cout << " ";
      }
    }
    std::cout << "\n";
  }
}

void create_matrix(int rows, int cols, int** matrix)
{
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      std::cin >> matrix[i][j];
    }
  }
}

int main()
{
  int size_matrix_i_line = 0;
  int size_matrix_j_column = 0;

  if (!(std::cin >> size_matrix_i_line >> size_matrix_j_column)) {
    return 1;
  }
  if (size_matrix_i_line <= 0 || size_matrix_j_column <= 0) {
    return 1;
  }

  int** matrix = allocate_matrix(size_matrix_i_line, size_matrix_j_column);
  if (matrix == nullptr) {
    return 2;
  }

  create_matrix(size_matrix_i_line, size_matrix_j_column, matrix);
  if (!std::cin) {
    free_matrix(matrix, size_matrix_i_line);
    return 1;
  }

  int** transposed = transport_matrix(matrix, size_matrix_i_line, size_matrix_j_column);
  if (transposed == nullptr) {
    free_matrix(matrix, size_matrix_i_line);
    return 2;
  }

  print_matrix(transposed, size_matrix_j_column, size_matrix_i_line);

  free_matrix(transposed, size_matrix_j_column);
  free_matrix(matrix, size_matrix_i_line);

  return 0;
}
