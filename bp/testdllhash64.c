#include <stdio.h>
#include <stdlib.h>

#include "dll.h"
#include "hashtable.h"
#include "dllhashtable.h"


#define NITEMS 10000
#define NSLOTS 32
#define MAXITEMS 10000000

int value_tab[MAXITEMS] = {0};

unsigned int hashfunc(struct HASHTABLE* hashtable_ptr, void* key)
{
	unsigned int hash;
	int nbits;
	unsigned int nslots;

	hash = fnv_32a_buf(key, 8, FNV1_32A_INIT);
	nslots = hashtable_ptr->nslots;
	if (hashtable_is2power(nslots)){
		nbits = hashtable_log2(nslots);
		hash = (hash >> nbits) & (hash & ((1 << nbits) - 1));
	}
	else {
		/* lazy mod without retry */
		hash %= nslots;
	}
	return hash;
}

int compfunc(char* l, char* r)
{
	// int i;
	// for (i = 0; i < 8; i++){
	// 	if (*l++ != *r++){
	// 		return 1;
	// 	}
	// }
	int a,b;
	a = *(unsigned*)l - *(unsigned*)r;
	b = *(unsigned*)(l+4) - *(unsigned*)(r+4);
	if (a == 0 && b == 0){
		return 0 ;
	}
	else {
		return 1;
	}
}


int main(int argc, char** argv)
{
	struct DLLHASHTABLE* dllhashtable_ptr;
	struct DLLHASHTABLE_HASHTABLEVALUE* hashtablevalue_ptr;
	int i, j; 
	char* key;
	int* value;
	int nitems, nslots;

	nitems = NITEMS;
	nslots = NSLOTS;

	if (argc == 3){
		nitems = atoi(argv[1]);
		nslots = atoi(argv[2]);
	}
	
	dllhashtable_ptr = dllhashtable_create(
		nslots,
		(HASHTABLE_COMPFUNC)compfunc,
		hashfunc,
		free);

	// printf("insert:\n");
	for (i = 0; i < nitems; i++){
		key = (char*)malloc(8);
		value = (int*)malloc(sizeof(int));
		*(int*)key = i;
		*(int*)(key+4) = i;
		*value = i;
		dllhashtable_insert(dllhashtable_ptr, key, value);
	}

	// printf("traverse:\n");
	hashtablevalue_ptr = NULL;

	for (i = 0; i < nitems; i++){
		hashtablevalue_ptr = dllhashtable_next(
			dllhashtable_ptr, hashtablevalue_ptr);
		// printf("hashtablevalue_ptr = %p\n", 
		// 	hashtablevalue_ptr);
		j = *(int*)hashtablevalue_ptr->value;
		value_tab[j]++;
		if (value_tab[j] > 1){
			printf("value_tab[j] > 1\n");
			exit(1);
		}
		if (j < 0 || j >= nitems){
			printf("j < 0 || j >= nitems\n");
			exit(1);
		}
	}

	key = (char*)malloc(8);
	for (i = 0; i < nitems; i++){
		*(int*)key = i;
		*(int*)(key+4) = i;
		dllhashtable_delete(dllhashtable_ptr, key);
	}

	if (dllhashtable_next(dllhashtable_ptr, NULL) != NULL){
		printf("the dllhashtable is not empty!\n");
		exit(1);
	}

	return 0;
}

