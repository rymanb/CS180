/* File header
 * ===========
 * file:       memmgr.c
 * author:     Ryman Barnett
 * email:      ryman.b@digipen.edu
 * course:     CS225
 * section:    A
 * assignment: 5
 * date:       11/11/2022
 *
 * file content:
 * this is a source file for memory manager functions to create, 
 * destroy manager, allocate, and free memory, requires memmgr.h
 */

#include "memmgr.h" /* memory manager header file */
#include <stdlib.h> /* malloc, free               */
#include <string.h> /* strcpy                     */
#include <stdio.h>  /* printf                     */

/* 
* @brief policy the memory manager uses to allocate memory
*/
static MMPolicy mPolicy;

/*!
 * @brief
 *  creates a memory manager
 *
 * @param bytes
 *  bytes the memory manager should allocate
 * 
 * @param policy
 *  first fit or best fit policy to use
 *
 * @return
 *  pointer to the memory manager, NULL if error
 */
block_info *mm_create(size_t bytes, enum MMPolicy policy)
{
    // allocate heap memory for the memory manager
    block_info *heap = (block_info *)malloc(sizeof(block_info));
    if (heap)
    {
        // set memory manager values
        heap->allocated = false;
        heap->size = bytes;
        heap->address = (char*)malloc(bytes);
        strncpy(heap->label, "START", LABEL_SIZE);
        heap->next = NULL;

        // set policy
        mPolicy = policy;

        return heap;
    }

    return NULL; // error
}

/*!
 * @brief
 *  allocates a block of memory
 *
 * @param heap
 *  pointer to the memory manager
 * 
 * @param bytes
 *  bytes to allocate
 * 
 * @param label
 *  label to give the block
 *
 * @return
 *  pointer to the block, NULL if error
 */
void *mm_allocate(block_info *heap, size_t bytes, const char *label)
{
    block_info *current = heap;        // walk heap, and hold found block
    block_info *availibleBlock = NULL; // hold the during find block
    block_info *newBlock = NULL;       // hold the new block
    size_t bestSize = 0;               // hold the best size found

    // find the first block that is big enough
    while (current != NULL)
    {
        // if the block is not allocated and is big enough
        if (current->allocated == false)
        {
            // if the block is big enough
            if (current->size >= bytes)
            {
                // if the block is the first block that is big enough

                if (availibleBlock == NULL || current->size < bestSize)
                {
                    // set the best block to the current block
                    availibleBlock = current;
                    bestSize = current->size;

                    // if the policy is first fit, no need to keep looking
                    if (mPolicy == mmpFirstFit)
                    {
                        break;
                    }
                }

            }
        }

        // move to the next block
        current = current->next;
    }

    // set the new block to the availible block
    current = availibleBlock;

    // make sure a block was found
    if (current != NULL)
    {
        // if the current block is big enough
        if (current->size > bytes)
        {
            // create a new block
            newBlock = (block_info *)malloc(sizeof(block_info));
            if (newBlock)
            {
                // set the new block's values to be the new free space
                newBlock->allocated = false;
                newBlock->size = current->size - bytes;
                newBlock->address = current->address + bytes;
                strncpy(newBlock->label, "FREE", LABEL_SIZE - 1);
                newBlock->label[LABEL_SIZE - 1] = '\0';
                newBlock->next = current->next;

                // set the current block to the new block
                current->next = newBlock;
                current->size = bytes;
            }
        }
        else
        {
            // no need to create a new block, just use the current block
            strncpy(current->label, label, LABEL_SIZE - 1);
            current->label[LABEL_SIZE - 1] = '\0';
        }
            
        // set blocks values and return the address
        current->allocated = true;
        strncpy(current->label, label, LABEL_SIZE - 1);
        current->label[LABEL_SIZE - 1] = '\0';

        return current->address;
    }

    return NULL; // no block was found
}

int mm_deallocate(block_info *heap, void *address)
{
    block_info *current = heap; // walk heap
    block_info *previous = NULL; // hold the previous block

    // find the block that contains the address
    while (current != NULL)
    {
        // if the block contains the address
        if (current->address == address)
        {
            // if the block is allocated
            if (current->allocated == true)
            {
                // set the block to not allocated
                current->allocated = false;

                // if the previous block is not allocated
                if (previous != NULL && previous->allocated == false)
                {
                    // combine the blocks
                    previous->size += current->size;
                    previous->next = current->next;
                    free(current);
                    current = previous;
                }

                // if the next block is not allocated
                if (current->next != NULL && current->next->allocated == false)
                {
                    // combine the blocks
                    current->size += current->next->size;
                    block_info* next = current->next->next;
                    free(current->next);
                    current->next = next;
                }

                return SUCCESS; 
            }
        }

        // move to the next block
        previous = current;
        current = current->next;
    }

    return FAILURE;
}

/*!
 * @brief
 *  destroys the memory manager
 *
 * @param heap
 *  pointer to the memory manager
 */
void mm_destroy(block_info *heap)
{
    block_info *current = heap; // walk heap
    block_info *next = NULL;    // hold the next block

    free(heap->address); // free the heap address string

    // free all the blocks
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}
