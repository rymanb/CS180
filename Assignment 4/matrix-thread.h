/*!
 * @file    matrix-thread.h
 * @author  Ryman Barnett
 * @par     email: ryman.b\@digipen.edu
 * @par     DigiPen login: ryman.b
 * @par     Course: CS180
 * @par     Section: A
 * @par     Assignment #4
 * @date    2022-10-14
 * @brief
 *        header file for matrix_thread function to calculate the
 *        value of a single position in a matrix squared and data
 *        struct for the thread.
 */

/*!
 * @brief
 *  struct containing the data for the thread
 */
typedef struct
{
  int *input;  /*!< pointer to the input matrix  */
  int* output; /*!< pointer to the output matrix */
  int width;   /*!< width of the matrix          */
  int row;     /*!< row of the position          */
  int col;     /*!< column of the position       */ 
  int pos;     /*!< position in the matrix       */
}matrix_data;

/*!
 * @brief
 *  calculates the value of a single position in a matrix squared
 *
 * @param data
 *  struct containing the data for the thread
 *
 * @return
 *  null
 */
void* matrix_thread(void* data);
