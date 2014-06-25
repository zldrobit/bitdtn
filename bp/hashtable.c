#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

struct HASHTABLE* hashtable_create(unsigned int nslots, 
	HASHTABLE_COMPFUNC compfunc,
	HASHTABLE_HASHFUNC hashfunc,
	HASHTABLE_DELVALUEFUNC delvaluefunc)
{
	struct HASHTABLE* hashtable_ptr;
	unsigned int i;
	hashtable_ptr = (struct HASHTABLE*) 
		malloc(sizeof(unsigned int) + 
			sizeof(HASHTABLE_COMPFUNC) +
			sizeof(HASHTABLE_HASHFUNC) +
			nslots * sizeof(struct HASHITEM));
	hashtable_ptr->nr = 0;
	hashtable_ptr->nslots = nslots;
	hashtable_ptr->compfunc = compfunc;
	hashtable_ptr->hashfunc = hashfunc;
	hashtable_ptr->delvaluefunc = delvaluefunc;
	for (i = 0; i < nslots; i++){
		hashtable_ptr->items[i] = NULL;
	}
	return hashtable_ptr;
}

// void hashtable_destroy(struct HASHTABLE* hashtable_ptr)
// {
// 	free(hashtable_ptr);
// }

int hashtable_is2power(unsigned int n)
{
	/* n > 0 */
	int nones = 0;
	while (n>0){
		nones++;
		n &= (n-1);
	}
	if (nones > 1){
		return 0;
	}
	else {
		return 1;
	}
}

int hashtable_log2(unsigned int n)
{
	/* n > 0 */
	int exp = 0;
	while (n > 0){
		n >>= 1;
		exp++;
	}
	return exp-1;
}

// void hashtable_hashfunc(struct HASHTABLE* hashtable_ptr, void* key)
// {
// 	unsigned int hash;
// 	int nbits;
// 	unsigned int nslots;
// 
// 	hash = fnv_32a_buf(key, 8, FNV1_32A_INIT);
// 	nslots = hashtable_ptr->nslots;
// 	if (hashtable_is2power(nslots)){
// 		nbits = hashtable_log2(nslots);
// 		hash = (hash >> nbits) & (hash & ((1 << nbits) - 1));
// 	}
// 	else {
// 		/* lazy mod without retry */
// 		hash %= nslots;
// 	}
// 	return hash;
// }

struct HASHITEM* hashtable_insert(struct HASHTABLE* hashtable_ptr, 
	void* key, void* value)
{
	unsigned int hash;
	struct HASHTABLE* head_ptr = NULL;
	struct HASHITEM* item_ptr;
	HASHTABLE_HASHFUNC hashfunc;
	
	hashtable_ptr->nr++;
	hashfunc = hashtable_ptr->hashfunc;
	item_ptr = (struct HASHITEM*)malloc(sizeof(struct HASHITEM));
	item_ptr->key = key;
	item_ptr->value = value;
	item_ptr->next = NULL;

	// if (*(unsigned int*)item_ptr->key == 7){
	// 	printf("insert item_ptr->key = %u:%u\n", 
	// 		*(unsigned int*)item_ptr->key,
	// 		*(unsigned int*)(item_ptr->key+4));
	// }
	hash = hashfunc(hashtable_ptr, item_ptr->key);
	// printf("insert hash = %u \n", hash);
	// if (*(unsigned int*)item_ptr->key == 7){
	// 	printf("insert hash %u \n", hash);
	// }

	// head_ptr = hashtable_ptr->itmes[hash];	

	if (hashtable_ptr->items[hash] == NULL){
		hashtable_ptr->items[hash] = item_ptr;
	}
	else {
		item_ptr->next = hashtable_ptr->items[hash];
		hashtable_ptr->items[hash] = item_ptr;
	}

	return item_ptr;
}

void hashtable_delete(struct HASHTABLE* hashtable_ptr, void* key)
{
	unsigned int hash;
	struct HASHITEM* item_ptr;
	struct HASHITEM* prev_ptr = NULL;
	HASHTABLE_COMPFUNC compfunc; 
	HASHTABLE_HASHFUNC hashfunc;
	HASHTABLE_DELVALUEFUNC delvaluefunc;

	hashtable_ptr->nr--;
	// printf("hashtable_delete\n");
	hashfunc = hashtable_ptr->hashfunc;
	hash = hashfunc(hashtable_ptr, key);
	// printf("hash = %u\n", hash);
	item_ptr = hashtable_ptr->items[hash];
	// printf("item_ptr = 0x%X\n", (unsigned int)item_ptr);
	compfunc = hashtable_ptr->compfunc;
	delvaluefunc = hashtable_ptr->delvaluefunc;
	if (item_ptr == NULL){
		/* not found */
	}

	// printf("before while\n");

	while (compfunc(item_ptr->key, key) != 0){
		prev_ptr = item_ptr;
		item_ptr = item_ptr->next;
		// printf("item_ptr = 0x%X\n", (unsigned int)item_ptr);
	}
	/* assert item_ptr != NULL? */
	if (item_ptr == NULL){
		printf("error itme_ptr == NULL\n");
	}
	else if (item_ptr == hashtable_ptr->items[hash]){
		// printf("head ptr\n");
		hashtable_ptr->items[hash] = hashtable_ptr->items[hash]->next;
		free(item_ptr->key);
		// free(item_ptr->value);
		delvaluefunc(item_ptr->value);
		free(item_ptr);
	}
	else {
		// printf("middle ptr\n");
		prev_ptr->next = item_ptr->next;
		free(item_ptr->key);
		// free(item_ptr->value);
		delvaluefunc(item_ptr->value);
		free(item_ptr);
	}
}

struct HASHITEM* hashtable_search(struct HASHTABLE* hashtable_ptr, void* key)
{
	unsigned int hash;
	struct HASHITEM* item_ptr;
	struct HASHITEM* prev_ptr = NULL;
	HASHTABLE_COMPFUNC compfunc; 
	HASHTABLE_HASHFUNC hashfunc;

	// if (*(unsigned*)key == 7){
	// 	printf("hashtable_search key = %u:%u\n", 
	// 		*(unsigned int*)key,
	// 		*(unsigned int*)(key+4));
	// }
	hashfunc = hashtable_ptr->hashfunc;
	hash = hashfunc(hashtable_ptr, key);
	// if (*(unsigned*)key == 7){
	// 	printf("search hash = %u\n", hash);
	// }
	item_ptr = hashtable_ptr->items[hash];
	// printf("item_ptr = 0x%X\n", (unsigned int)item_ptr);
	compfunc = hashtable_ptr->compfunc;
	if (item_ptr == NULL){
		return NULL;
	}

	// printf("before while\n");

	while (item_ptr != NULL && compfunc(item_ptr->key, key) != 0){
		prev_ptr = item_ptr;
		item_ptr = item_ptr->next;
		// printf("item_ptr = 0x%X\n", (unsigned int)item_ptr);
	}
	/* assert item_ptr != NULL? */
	if (item_ptr == NULL){
		// printf("itme_ptr == NULL\n");
	}
	else if (item_ptr == hashtable_ptr->items[hash]){
		// printf("head ptr\n");
		// hashtable_ptr->items[hash] = 
		// 	 hashtable_ptr->items[hash]->next;
		// free(item_ptr);
	}
	else {
		// printf("middle ptr\n");
		// prev_ptr->next = item_ptr->next;
		// free(item_ptr);
	}
	return item_ptr;
}
