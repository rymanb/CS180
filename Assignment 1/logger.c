/*!
 * @file    logger.c
 * @author  Ryman Barnett
 * @par     email: ryman.b\@digipen.edu
 * @par     DigiPen login: ryman.b
 * @par     Course: CS180
 * @par     Section: A
 * @par     Assignment #1a
 * @date    2022-09-13
 * @brief
 *        Source file for logger functions to open and write to a file using
 *        buffered output that will dump to file when the buffer is full.
 *
 */

#include <stdio.h>    // printf, perror
#include <string.h>   // strcpy, strlen, memcpy
#include <stdlib.h>   // malloc, free
#include <fcntl.h>    // open, O_WRONLY, O_CREAT, O_TRUNC
#include <sys/stat.h> // S_IRUSR, S_IWUSR
#include <unistd.h>   // close, write, fsync
#include "logger.h"   // logger stuff

static unsigned char *LOG_BUFFER;  //!< The memory buffer
static unsigned int LOG_BUFSIZE;   //!< The size of the buffer
static unsigned int LOG_BYTECOUNT; //!< How many bytes in the buffer?
static char *LOG_FILENAME;         //!< Name of the file to write the log to
static int LOG_FH = -1;            //!< The file handle of the open file

/*!
 * @brief
 *  opens a file, and allocated a buffer for writting to it
 *
 * @param filename
 *  const char* to the name of the file to open
 *
 * @param buffersize
 *  unsigned int to size of buffer to allocate
 *
 * @return
 *  LOG_NOMEM if buffer cant be allocated, LOG_OPENFAIL if file not opened,
 *  and LOG_SUCCESS otherwise
 */
LOG_RESULT log_open(const char *filename, unsigned int buffersize)
{
  int len = (strlen(filename) + 1);         // length of filename
  LOG_BUFSIZE = buffersize;                 // store buffersize
  LOG_BYTECOUNT = 0;                        // Initial bytes read are 0
  LOG_FILENAME = (char *)malloc(len);       // allocate space for filename
  LOG_BUFFER = (char *)malloc(LOG_BUFSIZE); // allocate buffer memory

  // make sure both mallocs were successfull
  if (LOG_BUFFER == NULL || LOG_FILENAME == NULL)
  {
    free(LOG_BUFFER);   // free buffer if it was allocated
    free(LOG_FILENAME); // free filename if it was allocated

    return LOG_NOMEM; // memory not allocated
  }

  // copy file name
  strcpy(LOG_FILENAME, filename);

  // open file
  LOG_FH = open(LOG_FILENAME, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

  // validate opened file
  if (LOG_FH == -1)
  {
    free(LOG_FILENAME); // free filename if it was allocated
    free(LOG_BUFFER);   // free buffer if it was allocated

    return LOG_OPENFAIL; // file not opened
  }

  return LOG_SUCCESS; // no errors file was opened
}

/*!
 * @brief
 *  write contents of the buffer to the disk and then reset the buffer
 *
 * @return
 *  LOG_WRITEFAIL if buffer cant be written to disk, and LOG_SUCCESS otherwise
 */
LOG_RESULT log_flush(void)
{
  // try to write buffer to file
  if (LOG_BYTECOUNT == 0 || write(LOG_FH, LOG_BUFFER, LOG_BYTECOUNT))
  {
    // sync file
    if (fsync(LOG_FH) != -1)
    {
      LOG_BYTECOUNT = 0;  // reset buffer counter

      return LOG_SUCCESS; // success
    }
  }

  log_close(); // close for error

  return LOG_WRITEFAIL; // failed to write or could not sync file
}

/*!
 * @brief
 *  write bytes from a given string into the buffer and flush it to disk when
 *  buffer is full
 *
 * @param text
 *  const unsigned char* to text to write to file
 *
 * @return
 *  LOG_WRITEFAIL if buffer could'nt be written to file, and LOG_SUCCESS
 *  otherwise
 */
LOG_RESULT log_write(const unsigned char *text)
{
  long unsigned i = 0; // loop counter

  // for each letter in text
  while (i < strlen(text))
  {
    // check if it should flush
    if (LOG_BYTECOUNT < LOG_BUFSIZE)
    {
      // if not write the text char into buffer
      LOG_BUFFER[LOG_BYTECOUNT] = text[i];
      ++LOG_BYTECOUNT; // another byte read
      ++i; // move text buffer counter
    }
    else // if it should flush
    {
      // flush buffer into file
      if (log_flush() == LOG_WRITEFAIL)
      {
        log_close();

        return LOG_WRITEFAIL; // failed to write
      }
    }
  }

  // check if final buffer is full
  if (LOG_BYTECOUNT >= LOG_BUFSIZE)
  {
    // flush buffer into file
    if (log_flush() == LOG_WRITEFAIL)
    {
      log_close();

      return LOG_WRITEFAIL; // failed to write
    }
  }

  return LOG_SUCCESS; // succeded
}

/*!
 * @brief
 *  closes a file, deallocates all buffers , and resets counters
 *
 * @return
 *  LOG_WRITEFAIL if buffer could'nt be written to file, and LOG_SUCCESS
 *  otherwise
 */
LOG_RESULT log_close(void)
{
  // flush remaining buffer into file
  LOG_RESULT writeRes =  log_flush();

  close(LOG_FH);      // close file
  free(LOG_BUFFER);   // free buffer
  free(LOG_FILENAME); // free filename

  LOG_BUFFER = NULL;  // reset dangling pointer
  LOG_FH = -1;        // reset dangling pointer
  LOG_FILENAME = NULL;// reset dangling pointer
  LOG_BUFSIZE = 0;    // reset size counter
  LOG_BYTECOUNT = 0;  // reset bytes read

  return writeRes; // result of write
}

/*!
 * @brief
 *  dumps contents of the buffer to the command line
 */
void log_dump(void)
{
  printf("BUFF[");

  for (unsigned i = 0; i < LOG_BYTECOUNT; i++)
    printf("%c", LOG_BUFFER[i]);

  printf("]BUFF\n");
}
