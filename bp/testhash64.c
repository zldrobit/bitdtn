#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

#define NITEMS 		123456
#define NSLOTS		9876
#define MAXSLOTS	10000000

struct HASHITEM* ptr_tab[MAXSLOTS];

struct ITEM{
	char (*key)[8];
	int* value;
	struct ITEM* next;
};

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
	struct HASHTABLE* hashtable_ptr;
	unsigned int i;
	struct ITEM* item_ptr;
	char key[8];
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
		item_ptr->key = (char(*)[8]) malloc(8 * sizeof(char));
		*(unsigned int*)item_ptr->key = i;
		*(unsigned int*)((char*)item_ptr->key+4) = i;
		item_ptr->value = (int*) malloc(sizeof(int));
		*item_ptr->value = i;
		item_ptr->next = NULL;
		// ptr_tab[i] = (struct HASHITEM*)item_ptr;
		// printf("main item_ptr->key %u\n", (unsigned)item_ptr->key);
		ptr_tab[i] = hashtable_insert(
			hashtable_ptr, 
			item_ptr->key,
			item_ptr->value);
	}
	
	for (i = 0; i < nitems; i++){
		*(unsigned*)key = i;
		*(unsigned*)(key+4) = i;
		item_ptr = (struct ITEM*)hashtable_search(hashtable_ptr, key);
		if ((struct HASHITEM*)item_ptr != ptr_tab[i]){
			printf("error %d item_ptr != ptr_tab[%d]\n", i, i);
			printf("item_ptr = 0x%p, ptr_tab[%d] = 0x%p\n",
				item_ptr,
				i,
				ptr_tab[i]);
			printf("item_ptr->key = %u:%u\n",
				*(unsigned*)item_ptr->key,
				*(unsigned*)((char*)item_ptr->key+4));
			printf("ptr_tab[%d]->key= %u:%u\n",
				i,
				*(unsigned*)ptr_tab[i]->key,
				*(unsigned*)((char*)ptr_tab[i]->key+4));
			printf("compfunc = %d\n", 
				compfunc((char*)item_ptr->key, 
					ptr_tab[i]->key));
				
			exit(1);
		}
	}
	for (i = 0; i < nitems; i++){
		// printf("delete %d\n", i);
		*(unsigned*)key = i;
		*(unsigned*)(key+4) = i;
		hashtable_delete(hashtable_ptr, key);
	}

	for (i = 0; i < nitems; i++){
		*(unsigned*)key = i;
		*(unsigned*)(key+4) = i;
		item_ptr = (struct ITEM*)hashtable_search(hashtable_ptr, key);
		if (item_ptr != NULL){
			printf("error %d item != NULL\n", i);
			exit(1);
		}
	}
	return 0;
}

