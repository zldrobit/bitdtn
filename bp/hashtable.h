#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stdint.h>
#include "fnv/fnv.h"

#define HASHTABLE_MAXSLOTS UINT32_MAX

struct HASHITEM{
	void* key;
	void* value;
	struct HASHITEM* next;
};

struct HASHTABLE;

typedef int (*HASHTABLE_COMPFUNC)(void* l, void* r);
typedef unsigned int (*HASHTABLE_HASHFUNC)(struct HASHTABLE* hashtable_ptr,
	void* key);
typedef void (*HASHTABLE_DELVALUEFUNC)(void* value);

struct HASHTABLE{
	unsigned int nslots;
	HASHTABLE_COMPFUNC compfunc;
	HASHTABLE_HASHFUNC hashfunc;
	HASHTABLE_DELVALUEFUNC delvaluefunc;
	struct HASHITEM* items[1];
};



struct HASHTABLE* hashtable_create(unsigned int nslots, 
	HASHTABLE_COMPFUNC compfunc,
	HASHTABLE_HASHFUNC hashfunc,
	HASHTABLE_DELVALUEFUNC delvaluefunc);
// void hashtable_destroy(struct HASHTABLE* hashtable_ptr);
int hashtable_is2power(unsigned int n);
int hashtable_log2(unsigned int n);
void hashtable_hashfunc(struct HASHTABLE* hashtable_ptr, void* key);
struct HASHITEM* hashtable_insert(struct HASHTABLE* hashtable_ptr, 
	void* key, void* value);
void hashtable_delete(struct HASHTABLE* hashtable_ptr, void* key);
struct HASHITEM* hashtable_search(struct HASHTABLE* hashtable_ptr, 
	void* key);

#endif

