/*!
 * @file    cachelist.c
 * @author  Ryman Barnett
 * @par     email: ryman.b\@digipen.edu
 * @par     DigiPen login: ryman.b
 * @par     Course: CS180
 * @par     Section: A
 * @par     Assignment #1b
 * @date    2022-09-20
 * @brief
 *        Source file for cachelist functions to create a linked list of
 *        nodes that will be used to store, manipulate, and retrieve data
 *        using either caching or non-caching methods.
 */

#include <stdio.h>     /* printf             */
#include <stdlib.h>    /* malloc, free, exit */
#include <string.h>    /* strncpy            */
#include "cachelist.h" /* cachelist stuff    */

/*!
 * @brief
 *  Creates a new node
 *
 * @param value
 *  value to store in the node
 *
 * @param label
 *  label to store in the node
 *
 * @return
 *  pointer to the new node 
 */
static cl_node *make_node(int value, const char *label)
{
  /* Allocate the memory */
  cl_node *node = (cl_node *)malloc(sizeof(cl_node));

  if (!node)
  {
    printf("Can't allocate new node.\n");
    exit(1);
  }

  /* Set the initial values */
  node->value = value;
  node->next = NULL;
  /* Be sure not to overwrite memory */
  strncpy(node->label, label, LABEL_SIZE - 1);
  node->label[LABEL_SIZE - 1] = 0;
  return node;
}

/*!
 * @brief
 *  adds a new node to the end of the list
 *
 * @param list
 *  head of the linked list
 *
 * @param value
 *  value to store in the node
 * 
 * @param label
 *  label to store in the node
 *
 * @return
 *  head of the linked list
 */
cl_node *cl_add_end(cl_node *list, int value, const char *label)
{
  // store the head of the list for walking
  cl_node *temp = list;

  // if the list is empty, make a new node at front
  if (!list)
    return make_node(value, label);

  // walk to the end of the list
  while (temp->next != NULL)
    temp = temp->next;
    
  // add the new node to the end of the list
  temp->next = make_node(value, label);

  return list; // return the head of the list
}

/*!
 * @brief
 *  adds a new node to the front of the list
 *
 * @param list
 *  head of the linked list
 *
 * @param value
 *  value to store in the node
 * 
 * @param label
 *  label to store in the node
 *
 * @return
 *  head of the linked list
 */
cl_node *cl_add_front(cl_node *list, int value, const char *label)
{
  // make a new node with the given value and label
  cl_node *newNode = make_node(value, label);
  cl_node *temp = list; // store the head of the list for walking

  // put newNode at the front of the list
  list = newNode;
  list->next = temp; // point the new node to the old head of the list

  return list; // return the head of the list
}

/*!
 * @brief
 *  removes node with a given value from the list
 *
 * @param list
 *  head of the linked list
 * 
 * @param search_value
 *  value to search for in the list
 *
 * @return
 *  head of the linked list
 */
cl_node *cl_remove(cl_node *list, int search_value)
{
  cl_node *temp = list; // store the head of the list for walking

  // if the list is empty, nothing to remove
  if (!list)
    return list;

  // if the first node is the one to remove
  if (list->value == search_value)
  {
    list = list->next; // point the head of the list to the next node
    free(temp);        // free the memory of the removed node

    return list;       // return the new head of the list
  }

  // walk the list until we find the node to remove
  while (temp->next != NULL)
  {
    // if the next node is the one to remove
    if (temp->next->value == search_value)
    {
      cl_node *temp2 = temp->next; // store the node to remove
      temp->next = temp->next->next; // point current node to the next node
      free(temp2); // free the memory of the removed node

      break; // stop walking the list
    }

    temp = temp->next; // move to the next node
  }

  return list; // return the head of the list
}

/*!
 * @brief
 *  inserts a new node into the list before the node with a given value
 *
 * @param list
 *  head of the linked list
 * 
 * @param search_value
 *  value of node to insert before for in the list
 * 
 * @param value
 *  value to store in the node
 * 
 * @param label
 *  label to store in the node
 *
 * @return
 *  head of the linked list
 */
cl_node *cl_insert_before(cl_node *list, int search_value, int value,
                          const char *label)
{
  cl_node *temp = list; // store the head of the list for walking

  // if the list is empty, nothing to insert before
  if (!list)
    return list;

  // if the first node is the one to insert before
  if (list->value == search_value)
    list = cl_add_front(list, value, label); // add the new node to the front

  // walk the list until we find the node to insert before
  while (temp->next != NULL)
  {
    // if the next node is the one to insert before
    if (temp->next->value == search_value)
    {
      cl_node *newNode = make_node(value, label); // make a new node
      // insert the new node before the node and connect the list
      newNode->next = temp->next;
      temp->next = newNode; 

      break; // stop walking the list
    }

    temp = temp->next; // move to the next node
  }

  return list; // return the head of the list
}

/*!
 * @brief
 *  inserts a new node into the list after the node with a given value
 *
 * @param list
 *  head of the linked list
 * 
 * @param search_value
 *  value of node to insert after for in the list
 * 
 * @param value
 *  value to store in the node
 * 
 * @param label
 *  label to store in the node
 */
void cl_insert_after(cl_node *list, int search_value, int value,
                     const char *label)
{
  cl_node *temp = list; // store the head of the list for walking

  // walk the list until we hit end or find the node to insert after
  while (temp != NULL)
  {
    // if the node is the one to insert after
    if (temp->value == search_value)
    {
      cl_node *newNode = make_node(value, label); // make a new node
      // insert the new node after the node and connect the list
      newNode->next = temp->next;
      temp->next = newNode;

      break; // stop walking the list
    }

    temp = temp->next; // move to the next node
  }
}

/*!
 * @brief
 *  finds first occurrence of a node with a given value in the list
 *  and counts how many compares were required to find it
 *
 * @param list
 *  head of the linked list
 * 
 * @param search_value
 *  value to search for in the list
 * 
 * @param cache
 *  whether or not to use cacheing
 * 
 * @param compares
 *  to store the number of compares that occured
 *
 * @return
 *  head of the linked list
 */
cl_node *cl_find(cl_node *list, int search_value, bool cache, int *compares)
{
  cl_node *temp = list; // store the head of the list for walking
  *compares = 0;        // initialize the number of compares to 0

  // walk the list until we hit end or find the node
  while (temp != NULL)
  {
    *compares += 1; // another compare has been done

    // if the node is the one we are looking for
    if (temp->value == search_value)
    {
      // if we are caching, move the node to the front of the list
      if (cache)
      {
        // copy label of node to be moved
        char *label = (char*)malloc(LABEL_SIZE);
        strncpy(label, temp->label, LABEL_SIZE - 1);
        label[LABEL_SIZE - 1] = '\0'; // make sure the string is null terminated

        // remove the node from the list
        list = cl_remove(list, search_value);
        
        // add the node to the front of the list
        list = cl_add_front(list, search_value, label);

        free(label); // free the memory of the label
      }

      break; // stop walking the list
    }

    temp = temp->next; // move to the next node
  }

  return list; // return the head of the list
}

/*!
 * @brief
 *  frees the memory of the linked list
 *
 * @param list
 *  head of the linked list to free
 */
void cl_destroy(cl_node *list)
{
  cl_node *temp = list; // store the head of the list for walking

  // walk the list until we hit end
  while (temp != NULL)
  {
    // store the next node
    cl_node *nextNode = temp->next;
    free(temp); // free the current node
    temp = nextNode; // move next node to head
  }
}

/*!
 * @brief
 *  prints the linked list
 *
 * @param list
 *  head of the linked list to print
 */
void cl_dump(const cl_node *list)
{
  printf("==================\n");

  /* Print each value in the list */
  while (list)
  {
    printf("%4i: %s\n", list->value, list->label);
    list = list->next;
  }
}
