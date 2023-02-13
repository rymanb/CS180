/*!
 * @file    child-matrix.c
 * @author  Ryman Barnett
 * @par     email: ryman.b\@digipen.edu
 * @par     DigiPen login: ryman.b
 * @par     Course: CS180
 * @par     Section: A
 * @par     Assignment #3
 * @date    2022-10-14
 * @brief
 *        Source file for child-matrix functions to calculate the
 *        value of a single position in a matrix squared.
 */

#include <stdlib.h>   /* exit, atoi            */
#include <sys/shm.h>  /* shmat, shmdt          */
#include <stdio.h>    /* printf                */

/*!
 * @brief
 *  calculates the value of a single position in a matrix squared
 *
 * @param argc
 *  number of arguments
 * 
 * @param argv
 *  array of arguments
 *
 * @return
 *  0 on success, -1 error code on failure
 */
int main(int argc, char **argv) 
{

  int childNum;    /* what child am I       */
  int row;         /* which row am I        */
  int col;         /* which column am I     */
  int shmid;       /* shared memory id      */
  int* buffer;     /* buffer for shared mem */
  int i;           /* loop index            */
  int width;       /* width of the matrix   */
  int sum = 0;     /* sum of the row        */

  /* Check for the correct number of parameters */
  if (argc < 5)
  {
    printf("Insufficient parameters supplied %i\n", argc);
    exit(-1); /* exit with error -1 */
  }

  /* get the shared memory id */
  shmid = atoi(argv[1]);
  /* get the child number */
  childNum = atoi(argv[2]);
  /* get the row of the matrix to use*/
  row = atoi(argv[3]);
  /* get the column of the matrix to use*/
  col = atoi(argv[4]);

  /* attach shared memory */
  buffer = (int *) shmat(shmid, NULL, 0);
  /* if failed exit */
  if (buffer == (int*)-1)
  {
    perror("shmat");
    exit (-1);        /* exit with error -1 */
  }

  /* get the width of the matrix */
  width = *buffer;
  /* calculate the sum of the row */
  for (i = 0; i < width; i++)
  {
    sum += buffer[1 + (row * width) + i] * buffer[1 + col + (i * width)];
  }
  buffer[(childNum + 1) + width * width] = sum; /* set position in shared mem to answer */

  /* cleanup memory*/
  /* detach memory from child process */
  if(shmdt(buffer) == -1)
  {
    perror("shmdt");
    exit(-1);         /* exit with error -1 */
  }

  exit(0); /* exit with no error */
}
