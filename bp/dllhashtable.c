#include <stdio.h>
#include <stdlib.h>

#include "dllhashtable.h"

void dllhashtable_hashtable_delvaluefunc(void* value)
{
	struct DLLHASHTABLE_HASHTABLEVALUE* hashtablevalue_ptr;
	DLLHASHTABLE_INNER_DELVALUEFUNC delvaluefunc;

	hashtablevalue_ptr = (struct DLLHASHTABLE_HASHTABLEVALUE*)value;
	delvaluefunc = hashtablevalue_ptr->delvaluefunc;
	delvaluefunc(hashtablevalue_ptr->value);
	free(value);
}

void voidfunc(void* value)
{
	;
}

struct DLLHASHTABLE* dllhashtable_create(
	unsigned int nslots,
	HASHTABLE_COMPFUNC compfunc,
	HASHTABLE_HASHFUNC hashfunc,
	HASHTABLE_DELVALUEFUNC hashtable_delvaluefunc)
{
	struct DLLHASHTABLE* dllhashtable_ptr;
	struct DLL* dll_ptr;
	struct HASHTABLE* hashtable_ptr;

	dllhashtable_ptr = (struct DLLHASHTABLE*)malloc(
		sizeof(struct DLLHASHTABLE));
	dll_ptr = dll_create(voidfunc);
	hashtable_ptr = hashtable_create(nslots,
		compfunc,
		hashfunc,
		dllhashtable_hashtable_delvaluefunc);
	dllhashtable_ptr->dll_ptr = dll_ptr;
	dllhashtable_ptr->hashtable_ptr = hashtable_ptr;
	// dllhashtable_ptr->dll_delvaluefunc= dll_delvaluefunc;
	dllhashtable_ptr->hashtable_delvaluefunc = hashtable_delvaluefunc;
	return dllhashtable_ptr;
}

// void dllhashtable_destroy()
// {
// 	;
// }

void dllhashtable_insert(struct DLLHASHTABLE* dllhashtable_ptr,
	void* key, void* value)
{
	struct DLL* dll_ptr;
	struct HASHTABLE* hashtable_ptr;
	struct DLLHASHTABLE_DLLITEM* dllitem_ptr;
	struct DLLHASHTABLE_HASHTABLEVALUE* hashtablevalue_ptr;
	struct HASHITEM* hashitem_ptr;

	dll_ptr = dllhashtable_ptr->dll_ptr;
	hashtable_ptr = dllhashtable_ptr->hashtable_ptr;

	hashtablevalue_ptr = (struct DLLHASHTABLE_HASHTABLEVALUE*)
		malloc(sizeof(struct DLLHASHTABLE_HASHTABLEVALUE));
	hashtablevalue_ptr->value = value;
	hashtablevalue_ptr->dllitem_ptr = NULL;	
	hashtablevalue_ptr->delvaluefunc = 
		dllhashtable_ptr->hashtable_delvaluefunc;
	
	hashitem_ptr = hashtable_insert(hashtable_ptr, key, 
		hashtablevalue_ptr);
	// printf("hashitem_ptr = %p\n", hashitem_ptr);
	// printf("hashtablevalue_ptr = %p\n", hashtablevalue_ptr);
	
	dllitem_ptr = (struct DLLHASHTABLE_DLLITEM*)
		dll_insert(dll_ptr, hashitem_ptr);
	// printf("dllitem_ptr = %p\n", dllitem_ptr);

	hashtablevalue_ptr->dllitem_ptr = dllitem_ptr;
}

void dllhashtable_delete(struct DLLHASHTABLE* dllhashtable_ptr,
	void* key)
{
	struct DLL* dll_ptr;
	struct HASHTABLE* hashtable_ptr;
	struct HASHITEM* hashitem_ptr;
	struct DLLHASHTABLE_HASHTABLEVALUE* hashtablevalue_ptr;
	struct DLLHASHTABLE_DLLITEM* dllitem_ptr;

	dll_ptr = dllhashtable_ptr->dll_ptr;
	hashtable_ptr = dllhashtable_ptr->hashtable_ptr;

	hashitem_ptr = hashtable_search(hashtable_ptr, key);
	hashtablevalue_ptr = hashitem_ptr->value;
	dllitem_ptr = hashtablevalue_ptr->dllitem_ptr;

	dll_delete(dll_ptr, (struct DLLITEM*)dllitem_ptr);
	hashtable_delete(hashtable_ptr, key);
}

struct DLLHASHTABLE_HASHTABLEVALUE* dllhashtable_next(
	struct DLLHASHTABLE* dllhashtable_ptr,
	struct DLLHASHTABLE_HASHTABLEVALUE* hashtablevalue_ptr)
{
	struct DLL* dll_ptr;
	struct DLLHASHTABLE_DLLITEM* dllitem_ptr;
	struct DLLHASHTABLE_DLLITEM* dllitem_next_ptr;
	struct DLLHASHTABLE_HASHTABLEVALUE* hashtablevalue_next_ptr;
	struct HASHITEM* hashitem_ptr;
	
	dll_ptr = dllhashtable_ptr->dll_ptr;
	if (hashtablevalue_ptr == NULL){
		dllitem_ptr = (struct DLLHASHTABLE_DLLITEM*)dll_ptr->head;
	}
	else {
		dllitem_ptr = hashtablevalue_ptr->dllitem_ptr;
	}

	dllitem_next_ptr = (struct DLLHASHTABLE_DLLITEM*)
		dll_next(dll_ptr, (struct DLLITEM*)dllitem_ptr);

	if (dllitem_next_ptr == NULL){
		/* no more item in dll */
		hashtablevalue_next_ptr = NULL;
	}
	else {
		hashitem_ptr = dllitem_next_ptr->hashitem_ptr;
		hashtablevalue_next_ptr = hashitem_ptr->value;
	}

	return hashtablevalue_next_ptr;
}


