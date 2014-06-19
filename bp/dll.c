#include <stdio.h>
#include <stdlib.h>

#include "dll.h"

/* Doubly Linked List */

struct DLL* dll_create(DLL_DELVALUEFUNC delvaluefunc)
{
	struct DLL* dll_ptr;
	struct DLLITEM* item_ptr;

	dll_ptr = (struct DLL*)malloc(sizeof(struct DLL));
	item_ptr = (struct DLLITEM*)malloc(sizeof(struct DLLITEM));
	item_ptr->left = item_ptr->right = item_ptr;
	dll_ptr->head = item_ptr;
	dll_ptr->delvaluefunc = delvaluefunc;
	return dll_ptr;
}

// void dll_destroy()
// {
// 	;
// }

int dll_isempty(struct DLL* dll_ptr)
{
	struct DLLITEM* head;

	head = dll_ptr->head;
	if (head->left == head && head->right == head){
		return 1;
	}
	return 0;
}

struct DLLITEM* dll_insert(struct DLL* dll_ptr, void* value)
{
	struct DLLITEM* head;
	struct DLLITEM* item_ptr;
	
	// printf("dll_insert value=%p\n", value);
	item_ptr = (struct DLLITEM*)malloc(sizeof(struct DLLITEM));
	item_ptr->value = value;
	item_ptr->left = item_ptr->right = NULL;

	head = dll_ptr->head;
	item_ptr->right = head->right;
	item_ptr->left = head;
	head->right->left = item_ptr;
	head->right = item_ptr;

	return item_ptr;
}

void dll_delete(struct DLL* dll_ptr, struct DLLITEM* item_ptr)
{
	struct DLLITEM* head;
	DLL_DELVALUEFUNC delvaluefunc;

	head = dll_ptr->head;
	delvaluefunc = dll_ptr->delvaluefunc;
	if (dll_isempty(dll_ptr)){
		/* empty list */
	}
	else {
		item_ptr->left->right = item_ptr->right;
		item_ptr->right->left = item_ptr->left;
		/* destroy value first */
		// free(item_ptr->value);
		delvaluefunc(item_ptr->value);
		free(item_ptr);
	}
}

struct DLLITEM* dll_next(struct DLL* dll_ptr, struct DLLITEM* item_ptr)
{
	struct DLLITEM* head;
	struct DLLITEM* item_next_ptr;

	head = dll_ptr->head;
	if (item_ptr == NULL){
		item_ptr = head;
	}
	item_next_ptr = item_ptr->right;
	if (item_next_ptr == head){
		return NULL;
	}
	return item_next_ptr;
}

