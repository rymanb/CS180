#include <stdbool.h> /* bool */

/* Max length of labels, including NUL character */
#define LABEL_SIZE 16

/* Cache list node */
typedef struct cl_node
{
	char label[LABEL_SIZE];
	int value;
	struct cl_node* next;
}cl_node;

/* Prototypes */
cl_node *cl_add_end(cl_node *list, int value, const char *label);
cl_node *cl_add_front(cl_node *list, int value, const char *label);
cl_node *cl_remove(cl_node *list, int search_value);
cl_node *cl_insert_before(cl_node *list, int search_value, int value, const char *label);
void cl_insert_after(cl_node *list, int search_value, int value, const char *label);
cl_node *cl_find(cl_node *list, int search_value, bool cache, int *compares);
void cl_destroy(cl_node *list);

/* Provided for you. DO NOT modify the code in cachelist.c */
void cl_dump(const cl_node *list);
