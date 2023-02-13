/*!
 * @file    main-thread.c
 * @author  Ryman Barnett
 * @par     email: ryman.b\@digipen.edu
 * @par     DigiPen login: ryman.b
 * @par     Course: CS180
 * @par     Section: A
 * @par     Assignment #4
 * @date    2022-10-14
 * @brief
 *        Source file for main-matrix functions to create a 
 *        matrix from a file, print a matrix, and run a matrix.
 *        into child threads that calculate matrix squared
 */

#include <stdio.h>         /* printf, fopen, fclose, fscanf */
#include <stdlib.h>        /* exit, malloc                  */
#include <pthread.h>       /* thread create/join/exit       */
#include "matrix-thread.h" /* thread data struct, thread fn */

/*!
 * @brief
 *  creates a matrix from a file
 *
 * @param filename
 *  name of file to read from
 * 
 * @param width
 *  width of the matrix
 *
 * @return
 *  pointer to the matrix
 */
int *get_matrix(const char *filename, int *width)
{
  int value, *matrix, result;
  FILE *fp;
  
  /* Open the file */
  fp = fopen(filename, "rt");
  if (!fp)
  {
    printf("Can't open file: %s\n", filename);
    exit(-1);
  }
  
  /* Read the width */
  result = fscanf(fp, "%d", width);
  if (result == -1)
  {
    printf("Can't read from file: %s\n", filename);
    fclose(fp);
    exit(-1);
  }

  /* Allocate the matrix */
  matrix = malloc(*width * *width * sizeof(int));
  if (!matrix)
  {
    printf("Can't malloc matrix\n");
    fclose(fp);
    exit (-2);
  }
  
  /* Read the matrix */
  while (!feof(fp))
  {
    result = fscanf(fp, "%d", &value);
    if (result == -1)
      break;
    *matrix++ = value;
  }
  fclose(fp); /* close the file */
  return matrix - (*width * *width); /* return the matrix */
}

/*!
 * @brief
 *  prints a matrix
 *
 * @param matrix
 *  pointer to the matrix
 *
 * @param width
 *  width of the matrix
 */
void print_matrix(int *matrix, int width)
{
  int i, size = width * width;
  for (i = 0; i < size; i++)
  {
    printf("%8i", matrix[i]);
    if ( (i + 1) % width == 0)
      printf("\n");
  }
  printf("\n");
}

/*!
 * @brief
 *  handles creating a matrix, setting up buffers, and creating
 *  a thread for each row of the matrix to calculate the 
 *  square of the matrix
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
  int i;
  int width;                 /* width of the matrix           */
  int matsize;               /* total matrix values           */
  int *input_matrix;         /* the matrix read in            */
  int *result_matrix;        /* threads will put results here */

  
  /* check for correct number of arguments */
  if (argc < 2)
  {
    printf("Insufficient parameters supplied\n");
    return -1;
  }
  
  /* Reading the input matrix from a file into it's memory. */
  input_matrix = get_matrix(argv[1], &width);

  /* calculate the size of the matrix */
  matsize = width * width;

  /* Allocating memory for the result matrix. */ 
  result_matrix = malloc(matsize * sizeof(int));
  if (!result_matrix)
  {
    printf("Can't malloc result matrix\n");
    free(input_matrix);
    exit(-1);
  }
  
  /* Printing the input matrix. */
  print_matrix(input_matrix, width);

  pthread_t threads[matsize]; /* array of threads */
  matrix_data ds[matsize];    /* array of data structs */
  
  /* Creating all of the other threads and supplying them with */
  /* their parameters                                          */ 
  for (i = 0; i < matsize; i++)
  {
    /* create the data struct for the thread */
    ds[i].input = input_matrix;
    ds[i].output = result_matrix;
    ds[i].width = width;
    ds[i].row = i / width;
    ds[i].col = i % width;
    ds[i].pos = i;

  /* create the thread */
    pthread_create(&threads[i], NULL, matrix_thread, &ds[i]);
  }

  /* Waiting for all of the threads to finish. */
  for (i = 0; i < matsize; i++)
  {
    pthread_join(threads[i], NULL);
  }
    
  /* Printing the resulting squared matrix. */
  print_matrix(result_matrix, width);
  
  /* Cleaning up any memory or resources the main thread created. */
  free(input_matrix);
  free(result_matrix);
  
  return 0;
}
