#include <stdlib.h> // exit
#include <string.h> // strncpy
#include <stdio.h>  // printf
#include "logger.h"

static int DUMPFILE = 0; // Show file contents?
static int BUFFSIZE = 8; // Buffer size

static int RandomInt(int low, int high)
{
  int number;
  number = rand() % (high - low + 1) + low;
  return number;
}

static void dump_file(const char *filename)
{
  FILE *fp = fopen(filename, "rb");
  if (!fp)
  {
    perror("fopen");
    exit (1);
  }

  printf("FILE[");
  fflush(stdout);
    
    // Not very efficient, but OK for debugging
  while (!feof(fp))
  {
    unsigned char buff;
    size_t result = fread(&buff, sizeof(unsigned char), 1, fp);
    if (result != 1)
      break;
    printf("%c", buff);
  }
  printf("]FILE\n");
  fflush(stdout);
  fclose(fp);
}

static void check_result(LOG_RESULT result)
{
  if (result != LOG_SUCCESS)
  {
    printf("Error: ");
    switch (result)
    {
      case LOG_NOMEM: 
      printf("LOG_NOMEM (%i)\n", result);
      break;

      case LOG_OPENFAIL: 
      printf("LOG_OPENFAIL (%i)\n", result);
      break;

      case LOG_WRITEFAIL: 
      printf("LOG_WRITEFAIL (%i)\n", result);
      break;

      default: 
      printf("Unknown (%i)\n", result);
      break;
    }
    exit (1);
  }
}

static void test1(void)
{
  const char *filename = "log.txt";
  unsigned int bufsize = BUFFSIZE;
  
  printf("test1 ************************************************\n");
  printf("BUFFSIZE: %i\n", BUFFSIZE);
  
  printf("log_open =====================\n");
  check_result(log_open(filename, bufsize));
  log_dump();
  if (DUMPFILE) dump_file(filename);

  printf("log_write 5 bytes ============\n");
  check_result(log_write("Hello"));
  log_dump();
  if (DUMPFILE) dump_file(filename);

  printf("log_write 8 bytes ============\n");
  check_result(log_write("Goodbye"));
  log_dump();
  if (DUMPFILE) dump_file(filename);
  
  printf("log_write 4 bytes ============\n");
  check_result(log_write("Wave"));
  log_dump();
  if (DUMPFILE) dump_file(filename);
  
  printf("log_write 5 bytes ============\n");
  check_result(log_write("Done."));
  log_dump();
  if (DUMPFILE) dump_file(filename);

  printf("log_close ====================\n");
  check_result(log_close());
  if (DUMPFILE) dump_file(filename);
}

static void test2(void)
{
  const char *filename = "log.txt";

  printf("test2 ************************************************\n");
  printf("BUFFSIZE: %i\n", BUFFSIZE);

  printf("log_open =====================\n");
  check_result(log_open(filename, BUFFSIZE));
  log_dump();
  if (DUMPFILE) dump_file(filename);

  printf("log_write 12 bytes ===========\n");
  check_result(log_write("123456789012"));
  log_dump();
  if (DUMPFILE) dump_file(filename);

  printf("log_flush ====================\n");
  check_result(log_flush());
  log_dump();
  if (DUMPFILE) dump_file(filename);

  printf("log_close ====================\n");
  check_result(log_close());
  if (DUMPFILE) dump_file(filename);
}

static void test3(void)
{
  const char *filename = "log.txt";

  printf("test3 ************************************************\n");
  printf("BUFFSIZE: %i\n", BUFFSIZE);

  printf("log_open =====================\n");
  check_result(log_open(filename, BUFFSIZE));
  log_dump();
  if (DUMPFILE) dump_file(filename);

  for (int i = 'a'; i <= 'j'; i++)
  {
    char buff[2] = {0};
    buff[0] = i;
  
    printf("log_write 1 byte =============\n");
    check_result(log_write(buff));  
    log_dump();
    if (DUMPFILE) dump_file(filename);
  }
  
  printf("log_close ====================\n");
  check_result(log_close());
  if (DUMPFILE) dump_file(filename);
}

static void test4(void)
{
  const char *filename = "log.txt";

  printf("test4 ************************************************\n");
  printf("BUFFSIZE: %i\n", BUFFSIZE);

  printf("log_open =====================\n");
  check_result(log_open(filename, BUFFSIZE));
  log_dump();
  if (DUMPFILE) dump_file(filename);

  for (int i = 'a'; i <= 'e'; i++)
  {
    char buff[2] = {0};
    buff[0] = i;
  
    printf("log_write 1 byte =============\n");
    check_result(log_write(buff));  
    log_dump();
    if (DUMPFILE) dump_file(filename);

    printf("log_flush ====================\n");
    check_result(log_flush());
    log_dump();
    if (DUMPFILE) dump_file(filename);
  }
  
  printf("log_close ====================\n");
  check_result(log_close());
  if (DUMPFILE) dump_file(filename);
}

static void test5(void)
{
  const char *filename = "log.txt";
  unsigned int bufsize = BUFFSIZE;
  
  printf("test5 ************************************************\n");
  printf("BUFFSIZE: %i\n", BUFFSIZE);
  
  printf("log_open =====================\n");
  check_result(log_open(filename, bufsize));
  log_dump();
  if (DUMPFILE) dump_file(filename);

  printf("log_write 20 bytes ============\n");
  check_result(log_write("12345678901234567890"));
  log_dump();
  if (DUMPFILE) dump_file(filename);

  printf("log_write 8 bytes ============\n");
  check_result(log_write("12345678"));
  log_dump();
  if (DUMPFILE) dump_file(filename);
  
  printf("log_write 4 bytes ============\n");
  check_result(log_write("9012"));
  log_dump();
  if (DUMPFILE) dump_file(filename);
  
  printf("log_write 22 bytes ============\n");
  check_result(log_write("3456789012345678901234"));
  log_dump();
  if (DUMPFILE) dump_file(filename);

  printf("log_close ====================\n");
  check_result(log_close());
  if (DUMPFILE) dump_file(filename);
}

static void test6(void)
{
  const char *filename = "/tmpfoobar/xyz/*/\\log.txt";
  unsigned int bufsize = BUFFSIZE;
  
  printf("test6 ************************************************\n");
  printf("BUFFSIZE: %i\n", BUFFSIZE);
  
  printf("log_open =====================\n");
  check_result(log_open(filename, bufsize));
  log_dump();
  if (DUMPFILE) dump_file(filename);
}

static void test7(void)
{
  const char *filename = "log.txt";
  unsigned int bufsize = BUFFSIZE;
  
  printf("test7 ************************************************\n");
  printf("BUFFSIZE: %i\n", BUFFSIZE);

  char array[] = "Any sufficiently advanced technology is indistinguishable"
                 "from magic.";
  
  printf("log_open =====================\n");
  check_result(log_open(filename, bufsize));
  log_dump();
  if (DUMPFILE) dump_file(filename);

  int size = sizeof(array);

  for (int i = 0; i < 50; i++)
  {
    int left = 0; //RandomInt(0, size - 20);
    int len = RandomInt(1, size - left - 1);

    char buffer[200] = {0};
    strncpy(buffer, array + left, len);

    printf("log_write %2i bytes ============\n", len);
    check_result(log_write(buffer));
    log_dump();
    if (DUMPFILE) dump_file(filename);
  }

  printf("log_close ====================\n");
  check_result(log_close());
}

int main(int argc, char **argv)
{
  int test = 0;

  if (argc > 1)
    test = atoi(argv[1]);
  if (argc > 2)
    BUFFSIZE = atoi(argv[2]);
  if (argc > 3)
    DUMPFILE = atoi(argv[3]);

  typedef void (*TestFn)(void);
  TestFn tests[] = {test1, test2, test3, test4, test5, test6, test7};
  int size = sizeof(tests) / sizeof(*tests);

  if (test >= 1 && test <= size)
    tests[test - 1]();

  return 0;
}
