#ifndef MEMMGR_H
#define MEMMGR_H

#include <stddef.h> /* size_t */

#define LABEL_SIZE 16

typedef struct block_info
{
  int allocated;
  size_t size;
  char *address;
  char label[LABEL_SIZE];
  struct block_info *next;
}block_info;

#define false 0
#define true 1

#define SUCCESS 0
#define FAILURE -1

typedef enum MMPolicy {mmpFirstFit, mmpBestFit} MMPolicy;

block_info *mm_create(size_t bytes, enum MMPolicy policy);
void *mm_allocate(block_info *heap, size_t bytes, const char *label);
int mm_deallocate(block_info *heap, void *address);
void mm_destroy(block_info *heap);

#endif
