
 * @section DESCRIPTION
 *  class Matrix, includes constructors, copy and defaul const.
 *  includes methods such as get and set matrix dimensions
 *  and overloaded operators, =, *, +, +=, (), [], >>, <<, *=,/=
 */
#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>
#include <iostream>

/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims {
  int rows, cols;
} MatrixDims;

/**
 * This class represents the given Matrix for working with manipulations on
 * images
 */
class Matrix {

 private:
  MatrixDims _dims;
  float *_matrix;

 public:

  /**
   * Default constructor - Constructs 1*1 matrix, where the single
   * element is initiated to 0.
   */
  Matrix() noexcept;

  /**
   * Constructor - Constructs matrix rows * cols, Initiates all elements to 0.
   * @param rows num of rows in a matrix
   * @param cols num of cols in a matrix
   */
  Matrix(int rows, int cols) noexcept(false);

  /**
   * Copy constructor - Constructs matrix from another matrix.
   * @param m - given matrix to copy
   */
  Matrix(const Matrix &m) noexcept;

  /**
   * Matrix destructor - Destroys the matrix
   */
  ~Matrix() noexcept;

  /**
   * Returns the amount of rows (int).
   * @return num of row
   */
  int GetRows() const noexcept;

  /**
   * Returns the amount of columns (int).
   * @return num of columns
   */
  int GetCols() const noexcept;

  /**
   * Transforms a matrix into a column vector.
   * @return matrix as vector
   */
  Matrix &Vectorize() noexcept;

  /**
   * Prints matrix elements, Prints new line after each row
   */
  void Print() const noexcept;

  /**
   * Overloads assignment = operator by putting other matrix in the current one.
   * @param other - new matrix.
   * @return - deep copy of new matrix.
   */
  Matrix &operator=(const Matrix &other) noexcept;

  /**
   * Matrix multiplication
   * @param other - Matrix to multiply with.
   * @return Matrix multiplication product.
   */
  Matrix operator*(const Matrix &other) const noexcept(false);

  /**
   * Multiply current matrix with a scalar on the right
   * @param scalar - scalar
   * @return Matrix with all its elements multiplied by a scalar.
   */
  Matrix operator*(const float &scalar) const noexcept;

  /**
   * Multiply given matrix with a scalar on the left
   * @param matrix - Matrix to multiply with.
   * @param scalar - scalar
   * @return Matrix with all its elements multiplied by a scalar.
   */
  friend Matrix operator*(const float &scalar, const Matrix &matrix) noexcept;

  /**
   * @brief Matrix multiplication with assignment of the multiplication result
   * into current matrix
   * @param other the matrix to multiply on current matrix
   * @return reference to current matrix after the multiplication performed
   */
  Matrix &operator*=(const Matrix &other) noexcept(false);

  /**
   * @brief Scalar mult. accumulation
   * @param scalar given scalar
   * @return reference to current matrix after multiplied by given scalar
   */
  Matrix &operator*=(const float &scalar) noexcept;

  /**
   * @brief Scalar division on the right of the current matrix
   * @param scalar given scalar
   * @return matrix after division performed on current matrix
   */
  Matrix operator/(const float &scalar) const noexcept(false);

  /**
   * @brief Scalar division on current matrix with assigning the value
   * returned
   * @param scalar the given scalar
   * @return the current matrix after the division by given scalar
   */
  Matrix &operator/=(const float &scalar) noexcept(false);

  /**
   * Matrix addition
   * @param other Matrix to sum with.
   * @return New matrix with the solution of the addition
   */
  Matrix operator+(const Matrix &other) const noexcept(false);

  /**
   * Matrix addition accumulation
   * @param other - matrix to sum in.
   * @return current matrix summed with the other
   */
  Matrix &operator+=(const Matrix &other) noexcept(false);

  /**
   * @brief Matrix scalar addition.
   * @param scalar given scalar
   * @return current matrix after scalar added to it's each cell.
   */
  Matrix &operator+=(const float &scalar) noexcept;

  /**
   * Parenthesis indexing - Get element in 2D matrix.
   * @param row - row of an elements
   * @param col - col of an element
   * @return the element in the given row and col
   */
  float &operator()(int row, int col) noexcept(false);

  /**
   * Parenthesis indexing - Get element in 2D matrix. const version
   * @param row - row of an elements
   * @param col - col of an element
   * @return the element in the given row and col
   */
  float &operator()(int row, int col) const noexcept(false);

  /**
   * Brackets indexing -  Get element in 1D matrix.
   * @param index - index of element in the row in the matrix(when vectorized)
   * @return the element in the given row.
   */
  float &operator[](int index) noexcept(false);

  /**
   *  Brackets indexing -  Get element in 1D matrix. - const version
   * @param index - index of element in the row in the matrix(when vectorized)
   * @return the element in the given row.
   */
  float &operator[](int index) const noexcept(false);

  /**
   * @brief Equality - check if current matrix equals to the given matrix
   * @param other given matrix to compare to
   * @return true if current matrix equals the given matrix, else false
   */
  bool operator==(const Matrix &other) const noexcept;

  /**
   * @brief Not equal -check if current matrix not equals to the given matrix
   * @param other given matrix to compare to
   * @return true if current matrix not equals the given matrix, else false
   */
  bool operator!=(const Matrix &other) const noexcept;

  /**
   * Fills matrix elements. Reads from given input stream.into current matrix
   * @param is - Input stream.
   * @param matrix - Given matrix to fill
   * @return given stream.
   */
  friend std::istream &operator>>(std::istream &is,
                                  const Matrix &matrix) noexcept(false);

  /**
   * output stream : prints matrix into output stream
   * @param out - output stream.
   * @param matrix - Given matrix.
   * @return Output stream
   */
  friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);

};
#endif //MATRIX_H



