/*!
 * @file    parent-matrix.c
 * @author  Ryman Barnett
 * @par     email: ryman.b\@digipen.edu
 * @par     DigiPen login: ryman.b
 * @par     Course: CS180
 * @par     Section: A
 * @par     Assignment #3
 * @date    2022-10-14
 * @brief
 *        Source file for parent-matrix functions to create a 
 *        matrix from a file, print a matrix, and fork a matrix.
 *        into child processes that calculate matrix squared
 */

#include <stdio.h>    /* fopen, fscanf, fclose, printf */
#include <stdlib.h>   /* exit, malloc                  */
#include <sys/shm.h>  /* shmget, shmat, shmdt, shmctl  */
#include <unistd.h>   /* fork, execv                   */
#include <sys/wait.h> /* waitpid                       */

/*! max length for arg buffer for ints, 100 should be plenty */
#define ARG_MAX 100 

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
  int value, *matrix; /* array for matrix */
  FILE *fp;           /* file to open */
  
  /* Open the file in text/translated mode */  
  fp = fopen(filename, "rt");
  /* Check for error */
  if (!fp)
  {
    printf("Can't open file: %s\n", filename);
    exit(-1); /* exit with error -1 */
  }
  
  /* Read the width and allocate the matrix */
  fscanf(fp, "%d", width);
  matrix = (int*)malloc(*width * *width * sizeof(int));
  if (!matrix)
  {
    printf("Can't malloc matrix\n");
    fclose(fp); /* close the file */
    exit (-2);  /* exit with error -2 */
  }
  
  /* Read the vaules and put in the matrix */
  while (!feof(fp))
  {
    int result = fscanf(fp, "%d", &value);
    if (result == -1)
      break;
    *matrix++ = value;
  }
  fclose(fp); /* close the file */
  
  /* Return the address of the matrix */
  return matrix - (*width * *width);
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
 *  handles creating a matrix, setting up shared memory, and forking
 *  a child process for each row of the matrix to calculate the 
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
  int width;       /* width of the matrix           */
  int *matrix;     /* the matrix read in            */
  int *cPid;       /* child process id              */
  key_t key = 123; /* arbitrary key (0x7b)          */
  int shmid;       /* return value from fork/shmget */
  int* buffer;     /* buffer for shared mem         */
  int i;           /* loop index                    */
  size_t size;     /* size of shared memory         */

  /* Check for the correct number of parameters */
  if (argc < 3)
  {
    printf("Insufficient parameters supplied\n");
    return -1;
  }

  /* read in matrix values from file */ 
  matrix = get_matrix(argv[1], &width);
  
  /* print the matrix */
  print_matrix(matrix, width);

  /* allocate shared memory */
  size = (sizeof(int) * (width * width) * 2) + 1;
  shmid = shmget(key, size, 0600 | IPC_CREAT);
  /* if failed exit */
  if (shmid == -1)
  {
    perror("shmget");
    free (matrix); /* free the matrix */
    exit(-1);      /* exit with error -1*/
  }

  /* attach shared memory */
  buffer = (int *) shmat(shmid, NULL, 0);
  /* if failed exit */
  if (buffer == (int*)-1)
  {
    perror("shmat");
    free (matrix);                       /* free the matrix    */
    if(shmctl(shmid, IPC_RMID, 0) == -1) /* free memory block  */
    {
      perror("shmctl");
    }
    exit(-1);                            /* exit with error -1 */
  }

  /* copy the width and matrix into shared memory */
  buffer[0] = width;
  for (i = 1; i < (width * width) + 1; i++)
  {
    buffer[i] = matrix[i - 1];
  }

  /* create child process list */
  cPid = (int*)malloc(sizeof(int) * (width * width));
  
  /* Fork a child for each matrix entry and put into child array*/
  for (i = 0; i < width * width; i++)
  {
    /* fork a child */
    cPid[i] = fork();

    /* if child, exec the child program */
    if (cPid[i] == 0)
    {
        int res;              /* return value from execv              */
      char key[ARG_MAX];      /* shmid                                */
      char pos[ARG_MAX];      /* position in matrix of child          */
      char row[ARG_MAX];      /* row of child                         */
      char col[ARG_MAX];      /* column of child                      */
      char *args[6] = {"\0"}; /* arguments for child(Null terminated) */

      /*convert ints to strings for execution */
      sprintf(key, "%d", shmid);
      sprintf(pos, "%d", i);
      sprintf(row, "%d", i / width);
      sprintf(col, "%d", i % width);
      /* put arguments into array */
      args[0] = argv[2];
      args[1] = key;
      args[2] = pos;
      args[3] = row;
      args[4] = col;

      /* execute child */
      res = execv(argv[2], args);

      /* if execv fails */
      printf("execv failed with %d", res);
      free(cPid);   /* free the child pid list in the children */
      free(matrix); /* free the matrix                         */
      if(shmdt(buffer) == -1)
      {
        perror("shmdt");
      }
      exit(-1); /* exit child fail */
    }
  }

  /* wait for children */
  for (i = 0; i < width * width; i++)
  {
    int status; /* status of child */

    /* wait for all children */
    if(waitpid(cPid[i], &status, 0) == -1) 
    {
      perror("waitpid");
    }
  }
  
  /* print matrix from shared buffer */
  print_matrix(buffer + 1 + width * width, width); 
  
  /* cleanup memory                    */
  /* detach memory from parent process */
  if(shmdt(buffer) == -1)
  {
    perror("shmdt");
  }
  /* delete memory block */
  if(shmctl(shmid, IPC_RMID, 0) == -1) 
  {
    perror("shmctl");
  }
  free(cPid);   /* free child pid array */
  free(matrix); /* free the matrix      */

  return 0; /* parent completed success*/
}
