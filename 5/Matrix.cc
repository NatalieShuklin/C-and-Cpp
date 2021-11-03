
 *
 * @section DESCRIPTION
 *  class Matrix, includes constructors, copy and defaul const.
 *  includes methods such as get and set matrix dimensions
 *  and overloaded operators, =, *, +, +=, (), [], >>, <<, *=,/=
 */
// ------------------------------ includes ------------------------------

#include "Matrix.h"
#include "MatrixException.h"
#include <iostream>
// -------------------------- const definitions -------------------------
/**
 * @brief Check division by zero.
 */
#define ZERO_DIVISION "Division by zero.\n"
/**
 * @brief Check indexes are in valid ranges.
 */
#define OUT_OF_RANGE "Index out of range.\n"
/**
 * @brief Check dimensions valid for operation.
 */
#define INVALID_DIMENSIONS "Invalid matrix dimensions.\n"
/**
 * @brief Check input stream validity
 */
#define ERROR_STREAM "Error loading from input stream.\n"
/**
 * @brief used for default dimension of default constructor
 */
#define DIMENSION_ONE 1
/**
 * @brief non negative check
 */
#define NON_NEG 0
/**
 * @brief space for printing
 */
#define  SPACE " "
/**
 * @brief zero for comparing index
 */
#define ZERO 0

// ------------------------------ methods -----------------------------
/**
 * Default constructor - Constructs 1*1 matrix, where the single
 * element is initiated to 0.
 */
Matrix::Matrix() noexcept: Matrix(DIMENSION_ONE, DIMENSION_ONE) {}

/**
 * Constructor - Constructs matrix rows * cols, Initiates all elements to 0.
 * @param rows num of rows in a matrix
 * @param cols num of cols in a matrix
 */
Matrix::Matrix(int rows, int cols) noexcept(false): _dims{rows, cols} {
  if (rows <= NON_NEG || cols <= NON_NEG) {
    throw MatrixException(INVALID_DIMENSIONS);
  }
  _matrix = new float[rows * cols];
  for (int i = 0; i < _dims.rows; i++) {
    for (int j = 0; j < _dims.cols; j++) {
      _matrix[i * _dims.cols + j] = ZERO;
    }
  }
}

/**
 * Copy constructor - Constructs matrix from another matrix.
 * @param m - given matrix to copy
 */
Matrix::Matrix(const Matrix &m) noexcept: _dims{m.GetRows(), m.GetCols()} {
  _matrix = new float[m.GetRows() * m.GetCols()];
  for (int i = 0; i < m.GetRows(); i++) {
    for (int j = 0; j < m.GetCols(); j++) {
      _matrix[i * GetCols() + j] = m._matrix[i * GetCols() + j];
    }
  }
}

/**
 * Matrix destructor - Destroys the matrix
 */
Matrix::~Matrix() noexcept {
  delete[] _matrix;
}

/**
 * Returns the amount of rows (int).
 * @return num of row
 */
int Matrix::GetRows() const noexcept {
  return _dims.rows;
}

/**
 * Returns the amount of columns (int).
 * @return num of columns
 */
int Matrix::GetCols() const noexcept {
  return _dims.cols;
}

/**
 * Transforms a matrix into a column vector.
 * @return matrix as vector
 */
Matrix &Matrix::Vectorize() noexcept {
  _dims.rows = _dims.rows * _dims.cols;
  _dims.cols = DIMENSION_ONE;
  return *this;
}

/**
 * Prints matrix elements, Prints new line after each row
 */
void Matrix::Print() const noexcept {
  for (int i = 0; i < _dims.rows; i++) {
    for (int j = 0; j < _dims.cols; j++) {
      if (j != (_dims.cols - DIMENSION_ONE)) {
        std::cout << (*this)(i, j) << SPACE;
      }
    }
    if (i != (_dims.rows - DIMENSION_ONE)) {
      std::cout << std::endl;
    }
  }
}

// ------------------------------ operators -----------------------------
/**
 * Overloads assignment = operator by putting other matrix in the current one.
 * @param other - new matrix.
 * @return - deep copy of new matrix.
 */
Matrix &Matrix::operator=(const Matrix &rhs) noexcept {
  if (this == &rhs) {
    return *this;
  }
  delete[] _matrix;
  _dims.rows = rhs._dims.rows;
  _dims.cols = rhs._dims.cols;
  _matrix = new float[rhs.GetRows() * rhs.GetCols()];
  for (int i = 0; i < rhs.GetRows(); i++) {
    for (int j = 0; j < rhs.GetCols(); j++) {
      _matrix[GetCols() * i + j] = rhs._matrix[rhs.GetCols() * i + j];
    }
  }
  return *this;
}

/**
 * Matrix multiplication
 * @param other - Matrix to multiply with.
 * @return Matrix multiplication product.
 */
Matrix Matrix::operator*(const Matrix &rhs) const noexcept(false) {
  if (_dims.cols != rhs._dims.rows) {
    throw MatrixException(INVALID_DIMENSIONS);
  }
  Matrix mat_mult(_dims.rows, rhs._dims.cols);
  for (int i = 0; i < _dims.rows; i++) {
    for (int j = 0; j < rhs._dims.cols; j++) {
      for (int l = 0; l < _dims.cols; l++) {
        mat_mult(i, j) += (*this)(i, l) * rhs(l, j);
      }
    }
  }
  return mat_mult;
}

/**
 * Multiply current matrix with a scalar on the right
 * @param scalar - scalar
 * @return Matrix with all its elements multiplied by a scalar.
 */
Matrix Matrix::operator*(const float &scalar) const noexcept {
  return scalar * (*this);
}

/**
 * Multiply given matrix with a scalar on the left
 * @param matrix - Matrix to multiply with.
 * @param scalar - scalar
 * @return Matrix with all its elements multiplied by a scalar.
 */
Matrix operator*(const float &scalar, const Matrix &matrix) noexcept {
  Matrix sol_matrix(matrix);
  for (int i = 0; i < matrix.GetRows(); i++) {
    for (int j = 0; j < matrix.GetCols(); j++) {
      sol_matrix[i * matrix.GetCols() + j] =
          scalar * matrix[i * matrix.GetCols() + j];
    }
  }
  return sol_matrix;
}

/**
* @brief Matrix multiplication with assignment of the multiplication result
* into current matrix
* @param other the matrix to multiply on current matrix
* @return reference to current matrix after the multiplication performed
*/
Matrix &Matrix::operator*=(const Matrix &other) noexcept(false) {
  if (_dims.cols != other._dims.rows) {
    throw MatrixException(INVALID_DIMENSIONS);
  }
  Matrix mat_mult(_dims.rows, other._dims.cols);
  for (int i = 0; i < _dims.rows; i++) {
    for (int j = 0; j < other._dims.cols; j++) {
      for (int l = 0; l < _dims.cols; l++) {
        mat_mult(i, j) += (*this)(i, l) * other(l, j);
      }
    }
  }
  *this = mat_mult;
  return *this;
}

/**
 * @brief Scalar mult. accumulation
 * @param scalar given scalar
 * @return reference to current matrix after multiplied by given scalar
 */
Matrix &Matrix::operator*=(const float &scalar) noexcept {
  *this = scalar * (*this);
  return *this;
}

/**
 * @brief Scalar division on the right of the current matrix
 * @param scalar given scalar
 * @return matrix after division performed on current matrix
 */
Matrix Matrix::operator/(const float &scalar) const noexcept(false) {
  if (scalar == ZERO) {
    throw MatrixException(ZERO_DIVISION);
  }
  Matrix sol_matrix(*this);
  for (int i = 0; i < sol_matrix.GetRows(); i++) {
    for (int j = 0; j < sol_matrix.GetCols(); j++) {
      sol_matrix[i * sol_matrix.GetCols() + j] =
          sol_matrix[i * sol_matrix.GetCols() + j] / scalar;
    }
  }
  return sol_matrix;
}

/**
 * @brief Scalar division on current matrix with assigning the value
 * returned
 * @param scalar the given scalar
 * @return the current matrix after the division by given scalar
 */
Matrix &Matrix::operator/=(const float &scalar) noexcept(false) {
  if (scalar == ZERO) {
    throw MatrixException(ZERO_DIVISION);
  }
  for (int i = 0; i < GetRows(); i++) {
    for (int j = 0; j < GetCols(); j++) {
      _matrix[i * GetCols() + j] = _matrix[i * GetCols() + j] / scalar;
    }
  }
  return *this;
}

/**
* Matrix addition
* @param other Matrix to sum with.
* @return New matrix with the solution of the addition
*/
Matrix Matrix::operator+(const Matrix &rhs) const noexcept(false) {
  if (GetRows() != rhs.GetRows() || GetCols() != rhs.GetCols()) {
    throw MatrixException(INVALID_DIMENSIONS);
  }
  Matrix add_matrix(GetRows(), GetCols());
  for (int i = 0; i < GetRows(); i++) {
    for (int j = 0; j < GetCols(); j++) {
      add_matrix[i * GetCols() + j] = (*this)[i * GetCols() + j] +
          rhs[i * rhs.GetCols() + j];
    }
  }
  return add_matrix;
}

/**
 * Matrix addition accumulation
 * @param other - matrix to sum in.
 * @return current matrix summed with the other
 */
Matrix &Matrix::operator+=(const Matrix &rhs) noexcept(false) {
  if (GetRows() != rhs.GetRows() || GetCols() != rhs.GetCols()) {
    throw MatrixException(INVALID_DIMENSIONS);
  }
  (*this) = (*this) + rhs;
  return *this;
}

/**
 * @brief Matrix scalar addition.
 * @param scalar given scalar
 * @return current matrix after scalar added to it's each cell.
 */
Matrix &Matrix::operator+=(const float &scalar) noexcept {
  for (int i = 0; i < GetRows(); i++) {
    for (int j = 0; j < GetCols(); j++) {
      _matrix[i * GetCols() + j] = (*this)[i * GetCols() + j] + scalar;
    }
  }
  return *this;
}

/**
 * Parenthesis indexing - Get element in 2D matrix.
 * @param row - row of an elements
 * @param col - col of an element
 * @return the element in the given row and col
 */
float &Matrix::operator()(int row, int col) noexcept(false) {
  if (row >= _dims.rows || col >= _dims.cols || row < ZERO || col < ZERO) {
    throw MatrixException(OUT_OF_RANGE);
  }
  return _matrix[row * _dims.cols + col];
}

/**
 * Parenthesis indexing - Get element in 2D matrix. const version
 * @param row - row of an elements
 * @param col - col of an element
 * @return the element in the given row and col
 */
float &Matrix::operator()(int row, int col) const noexcept(false) {
  if (row < ZERO || col < ZERO || row >= _dims.rows || col >= _dims.cols) {
    throw MatrixException(OUT_OF_RANGE);
  }
  return _matrix[row * _dims.cols + col];
}

/**
 * Brackets indexing -  Get element in 1D matrix.
 * @param index - index of element in the row in the matrix(when vectorized)
 * @return the element in the given row.
 */
float &Matrix::operator[](int index) noexcept(false) {
  if (index < ZERO || index >= (_dims.rows * _dims.cols)) {
    throw MatrixException(OUT_OF_RANGE);
  }
  return _matrix[index];
}

/**
 * Brackets indexing -  Get element in 1D matrix. const version
 * @param index - index of element in the row in the matrix(when vectorized)
 * @return the element in the given row.
 */
float &Matrix::operator[](int index) const noexcept(false) {
  if (index < ZERO || index >= (_dims.rows * _dims.cols)) {
    throw MatrixException(OUT_OF_RANGE);
  }
  return _matrix[index];
}

/**
 * @brief Equality - check if current matrix equals to the given matrix
 * @param other given matrix to compare to
 * @return true if current matrix equals the given matrix, else false
 */
bool Matrix::operator==(const Matrix &other) const noexcept {
  if (GetCols() != other.GetCols() || GetRows() != other.GetRows()) {
    return false;
  }
  for (int i = 0; i < GetRows(); i++) {
    for (int j = 0; j < GetCols(); j++) {
      if (_matrix[i * GetCols() + j] != other[i * GetCols() + j]) {
        return false;
      }
    }
  }
  return true;
}

/**
 * @brief Not equal -check if current matrix not equals to the given matrix
 * @param other given matrix to compare to
 * @return true if current matrix not equals the given matrix, else false
 */
bool Matrix::operator!=(const Matrix &other) const noexcept {
  return !((*this) == other);
}

/**
 * Fills matrix elements. Reads from given input stream.into current matrix
 * @param is - Input stream.
 * @param matrix - Given matrix to fill
 * @return given stream.
 */
std::istream &operator>>(std::istream &is,
                         const Matrix &matrix) noexcept(false) {
  if (!is.good()) {
    throw MatrixException(ERROR_STREAM);
  }
  float value;
  int index = ZERO;
  while (is >> value) {
    matrix._matrix[index] = value;
    index++;
  }
  if (is.peek() != EOF) {
    throw MatrixException(ERROR_STREAM);
  }
  return is;
}

/**
 * output stream : prints matrix into output stream
 * @param out - output stream.
 * @param matrix - Given matrix.
 * @return Output stream
 */
std::ostream &operator<<(std::ostream &out, const Matrix &matrix) {
  for (int i = 0; i < matrix.GetRows(); i++) {
    for (int j = 0; j < matrix.GetCols(); j++) {
      if (j != (matrix.GetCols() - DIMENSION_ONE)) {
        out << matrix[i * matrix.GetCols() + j] << SPACE;
      } else {
        out << matrix[i * matrix.GetCols() + j];
      }
    }
    if (i != (matrix.GetRows() - DIMENSION_ONE)) {
      out << std::endl;
    }
  }
  return out;
}

