#ifndef LOGGER_H
#define LOGGER_H

typedef enum LOG_RESULT 
{
  LOG_SUCCESS,   // No errors
  LOG_NOMEM,     // Couldn't allocate buffer
  LOG_OPENFAIL,  // File couldn't be opened
  LOG_WRITEFAIL, // Disk write failed
}LOG_RESULT;

LOG_RESULT log_open(const char *filename, unsigned int buffersize);
LOG_RESULT log_write(const unsigned char* text);
LOG_RESULT log_flush(void);
LOG_RESULT log_close(void);
void log_dump(void);

#endif


// for acroread: sudo apt-get install libxml2:i386
