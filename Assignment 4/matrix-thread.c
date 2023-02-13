/*!
 * @file    matrix-thread.c
 * @author  Ryman Barnett
 * @par     email: ryman.b\@digipen.edu
 * @par     DigiPen login: ryman.b
 * @par     Course: CS180
 * @par     Section: A
 * @par     Assignment #4
 * @date    2022-10-14
 * @brief
 *        Source file for matrix_thread function to calculate the
 *        value of a single position in a matrix squared.
 */

#include <stdlib.h>        /* free                  */
#include "matrix-thread.h" /* matrix_thread         */

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
void* matrix_thread(void* data)
{
  matrix_data *ds = (matrix_data *)data; /* cast the data to the struct */
  int sum = 0;                           /* sum of the row              */
  int i;                                 /* loop index                  */

  /* calculate the sum of the row */
  for (i = 0; i < ds->width; i++)
  {

    sum += (ds->input)[(ds->row * ds->width) + i] * (ds->input)[ds->col + (i * ds->width)];
  }

  (ds->output)[ds->pos] = sum; /* set position in mem to answer */

  return NULL;
}
