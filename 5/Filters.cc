

 * @section DESCRIPTION
 *  filters operations for Matrix
 */
// ------------------------------ includes ------------------------------

#include "Filters.h"
#include <cmath>

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
/**
 * @brief pixels size in image
 */
#define PIXELS_SIZE 256
/**
 * @brief size one
 */
#define SIZE_ONE 1
/**
 * @brief vag of two numbers calculation
 */
#define AVG_DIVISION 2
/**
 * @brief convolution matrix size
 */
#define CONVOLUTION_SIZE 3
/**
 * @brief pixels last number
 */
#define PIXELS_LAST 255
/**
 * @brief NUMBER 1 used for matrix values
 */
#define ONE 1
/**
 * @brief number 2  used for matrix values
 */
#define TWO 2
/**
 * @brief number 4  used for matrix values
 */
#define FOUR 4
/**
 * @brief number 16  used for matrix values
 */
#define SIXTEEN 16
/**
 * @brief number   8  used for matrix values
 */
#define EIGHT 8
/**
 * @brief number -2  used for matrix values
 */
#define MINUS_TWO -2
/**
 * @brief number -1  used for matrix values
 */
#define MINUS_ONE -1

// ------------------------------ methods -----------------------------
/**
 * @brief Update values inside matrix to correct ones, if above 255 make it 255
 * or less than 0 -> update value to 0.
 * @param output matrix to update on
 */
void UpdateMatrixBoundaries(Matrix &output) {
  for (int i = 0; i < output.GetRows(); i++) {
    for (int j = 0; j < output.GetCols(); j++) {
      if (output(i, j) > PIXELS_LAST) {
        output(i, j) = PIXELS_LAST;
      } else if (output(i, j) < ZERO) {
        output(i, j) = ZERO;
      }
    }
  }
}

/**
 * @brief  performs quantization on a given matrix
 * @param image the matrix
 * @param avg_colors average colors to change the value of matrix cells
 * @param arr_colors range colors for color division of the matrix
 * @param arr_len array of avg_colors length
 */
void QuantizationMatrix(Matrix &quant, const Matrix &image, const int
avg_colors[], const int arr_colors[], int arr_len) {
  for (int i = 0; i < image.GetRows(); i++) {
    for (int j = 0; j < image.GetCols(); j++) {
      for (int k = 1; k < arr_len; k++) {
        if (image(i, j) <= (float) (arr_colors[k] - SIZE_ONE)
            && image(i, j) >= (float) (arr_colors[k - SIZE_ONE])) {
          quant(i, j) = (float) avg_colors[k - SIZE_ONE];
          break;
        }
      }
    }
  }
}

/**
 * @brief Performs quantization on the input image by the given number of
 * levels.
 * @param image he given image as matrix
 * @param levels given levels
 * @return Returns new matrix which is the result of running the operator on
 * the image.
 */
Matrix Quantization(const Matrix &image, int levels) {
  Matrix mat_quant(image.GetRows(), image.GetCols());
  int colors_per_level = PIXELS_SIZE / levels;
  int * arr_colors = new int[levels + SIZE_ONE];
  int count = ZERO;
  for (int i = 0; i < levels + SIZE_ONE; i++) {
    arr_colors[i] = count * colors_per_level;
    count++;
  }
  int *avg_colors = new int [levels];
  for (int j = 0; j < levels; j++) {
    avg_colors[j] = (arr_colors[j] + arr_colors[j + SIZE_ONE] - SIZE_ONE) /
        AVG_DIVISION;
  }
  QuantizationMatrix(mat_quant, image, avg_colors, arr_colors,
                     levels + SIZE_ONE);
  UpdateMatrixBoundaries(mat_quant);
  delete[] arr_colors;
  delete[] avg_colors;
  return mat_quant;
}

/**
 * @brief Creates the matrix around given cell
 * to be used for the convolution calculation, of matrix multiplication
 * @param image given image
 * @param middle_row given cells row index
 * @param middle_col given cells col index
 * @return a new matrix 3*3 with values from image, around the given cell
 * -cell is in the middle (1,1)
 */
Matrix CreateConvoMatrix(const Matrix &image, int
middle_row, int middle_col) {
  Matrix convo_mat(CONVOLUTION_SIZE, CONVOLUTION_SIZE);
  for (int j = 0; j < CONVOLUTION_SIZE; j++) {
    for (int i = 0; i < CONVOLUTION_SIZE; i++) {
      if (middle_col - SIZE_ONE + i < ZERO || middle_row - SIZE_ONE + j
          < ZERO || middle_col - SIZE_ONE +
          i >= image.GetCols() || middle_row - SIZE_ONE + j
          >= image.GetRows()) {
        convo_mat(j, i) = ZERO;
      } else {
        convo_mat(j, i) =
            image(middle_row - SIZE_ONE + j,
                  middle_col - SIZE_ONE + i);
      }
    }
  }
  return convo_mat;
}

/**
 * @brief performs convolution for given cell in matrix
 * @param output the output matrix
 * @param image the given input matrix
 * @param convolute matrix 3*3 surrounds the given cell in image matrix
 * @param middle_row row of the cell
 * @param middle_col col of the cell
 */
void PerformConvolutionCell(Matrix &output, const Matrix &image, const Matrix &
convolute, int middle_row, int middle_col) {
  Matrix convo = CreateConvoMatrix(image, middle_row, middle_col);
  float sum = ZERO;
  for (int i = 0; i < CONVOLUTION_SIZE; i++) {
    for (int j = 0; j < CONVOLUTION_SIZE; j++) {
      sum += convo(i, j) * convolute(i, j);
    }
  }
  output(middle_row, middle_col) = rintf(sum);
}

/**
 * @brief CREATES the matrix used for blurring calculations
 * @param blur  the given blur matrix to update with new values
 */
void CreateBlurringMat(Matrix &blur) {
  int col = ZERO;
  for (int i = 0; i < CONVOLUTION_SIZE; i++) {
    if (i == ZERO || i == TWO) {
      blur(i, col) = ONE;
      col++;
      blur(i, col) = TWO;
      col++;
      blur(i, col) = ONE;
    } else {
      blur(i, col) = TWO;
      col++;
      blur(i, col) = FOUR;
      col++;
      blur(i, col) = TWO;
    }
    col = ZERO;
  }
  blur *= ((float) ONE / SIXTEEN);
}

/**
 * @brief Performs gaussian blurring on the input image.
 * @param image given matrix
 * @return Returns new matrix which is the result of running the operator on
 * the image.
 */
Matrix Blur(const Matrix &image) {
  Matrix blur(CONVOLUTION_SIZE, CONVOLUTION_SIZE);
  Matrix output(image.GetRows(), image.GetCols());
  CreateBlurringMat(blur);
  for (int i = 0; i < image.GetRows(); i++) {
    for (int j = 0; j < image.GetCols(); j++) {
      PerformConvolutionCell(output, image, blur, i, j);
    }
  }
  UpdateMatrixBoundaries(output);
  return output;
}

/**
 * @brief CREATES SOBEL X MATRIX USED FOR sobel calculations
 * @return a new matrix represents a sobel x matrix
 */
Matrix CreateSobelX() {
  Matrix sobel(CONVOLUTION_SIZE, CONVOLUTION_SIZE);
  int col = ZERO;
  for (int i = 0; i < CONVOLUTION_SIZE; i++) {
    if (i == ZERO || i == TWO) {
      sobel(i, col) = ONE;
      col++;
      sobel(i, col) = ZERO;
      col++;
      sobel(i, col) = MINUS_ONE;
    } else {
      sobel(i, col) = TWO;
      col++;
      sobel(i, col) = ZERO;
      col++;
      sobel(i, col) = MINUS_TWO;
    }
    col = ZERO;
  }
  sobel *= ((float) ONE / EIGHT);
  return sobel;
}

/**
 * @brief CREATES SOBEL Y MATRIX USED FOR sobel calculations
 * @return a new matrix represents a sobel y matrix
 */
Matrix CreateSobelY() {
  Matrix sobel(CONVOLUTION_SIZE, CONVOLUTION_SIZE);
  int col = ZERO;
  for (int i = 0; i < CONVOLUTION_SIZE; i++) {
    if (i == ZERO) {
      sobel(i, col) = ONE;
      col++;
      sobel(i, col) = TWO;
      col++;
      sobel(i, col) = ONE;
    } else if (i == 2) {
      sobel(i, col) = MINUS_ONE;
      col++;
      sobel(i, col) = MINUS_TWO;
      col++;
      sobel(i, col) = MINUS_ONE;
    } else {
      sobel(i, col) = ZERO;
      col++;
      sobel(i, col) = ZERO;
      col++;
      sobel(i, col) = ZERO;
    }
    col = ZERO;
  }
  sobel *= ((float) ONE / EIGHT);
  return sobel;
}

/**
 * @brief Performs sobel edge detection on the input image
 * @param image given matrix image
 * @return Returns new matrix which is the result of running the operator on
 * the image
 */
Matrix Sobel(const Matrix &image) {
  Matrix output_x(image.GetRows(), image.GetCols());
  Matrix output_y(image.GetRows(), image.GetCols());
  Matrix sobel_x = CreateSobelX();
  Matrix sobel_y = CreateSobelY();
  for (int i = 0; i < image.GetRows(); i++) {
    for (int j = 0; j < image.GetCols(); j++) {
      PerformConvolutionCell(output_x, image, sobel_x, i, j);
      PerformConvolutionCell(output_y, image, sobel_y, i, j);
    }
  }
  Matrix final_sol = output_x + output_y;
  UpdateMatrixBoundaries(final_sol);
  return final_sol;
}
