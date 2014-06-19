#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

#define NITEMS 		123456
#define NSLOTS		9876
#define MAXSLOTS	10000000

void* ptr_tab[MAXSLOTS];

struct ITEM{
	int* key;
	int* value;
	struct ITEM* next;
};

unsigned int hashfunc(struct HASHTABLE* hashtable_ptr, void* key)
{
	unsigned int hash;
	int nbits;
	unsigned int nslots;

	hash = fnv_32a_buf(key, 4, FNV1_32A_INIT);
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

int compfunc(int* l, int* r)
{
	return *l - *r;
}

int main(int argc, char** argv)
{
	struct HASHTABLE* hashtable_ptr;
	int i;
	struct ITEM* item_ptr;
	int key;
	int nitems, nslots;

	nitems = NITEMS;
	nslots = NSLOTS;
	if (argc == 3){
		nitems = atoi(argv[1]);
		nslots = atoi(argv[2]);
	}

	hashtable_ptr = hashtable_create(nslots, 
		(HASHTABLE_COMPFUNC) compfunc, 
		hashfunc,
		free);
	for (i = 0; i < nitems; i++){
		item_ptr = (struct ITEM*) malloc(sizeof(struct ITEM));
		// printf("insert %d: ptr = 0x%X\n", i, (unsigned int)item_ptr);
		item_ptr->key = (int*) malloc(sizeof(int));
		item_ptr->value = (int*) malloc(sizeof(int));
		*item_ptr->key = i;
		*item_ptr->value = i;
		item_ptr->next = NULL;
		// ptr_tab[i] = item_ptr;
		ptr_tab[i] = 
			hashtable_insert(
				hashtable_ptr, 
				item_ptr->key,
				item_ptr->value);
	}
	
	for (i = 0; i < nitems; i++){
		item_ptr = (struct ITEM*)hashtable_search(hashtable_ptr, &i);
		if (item_ptr != ptr_tab[i]){
			printf("error %d item_ptr != ptr_tab[%d]\n", i, i);
			exit(1);
		}
	}
	for (i = 0; i < nitems; i++){
		// printf("delete %d\n", i);
		hashtable_delete(hashtable_ptr, &i);
	}

	for (i = 0; i < nitems; i++){
		item_ptr = (struct ITEM*)hashtable_search(hashtable_ptr, &i);
		if (item_ptr != NULL){
			printf("error %d item != NULL\n", i);
			exit(1);
		}
	}
	return 0;
}

