#ifndef _DLLHASHTABLE_H_
#define _DLLHASHTABLE_H_

#include "dll.h"
#include "hashtable.h"

typedef void (*DLLHASHTABLE_INNER_DELVALUEFUNC)(void* value);

struct DLLHASHTABLE{
	unsigned int nr;
	struct DLL* dll_ptr;
	struct HASHTABLE* hashtable_ptr;
	DLLHASHTABLE_INNER_DELVALUEFUNC hashtable_delvaluefunc;
};

struct DLLHASHTABLE_DLLITEM{
	struct HASHITEM* hashitem_ptr;
	struct DLLHASHTABLE_DLLITEM* left;
	struct DLLHASHTABLE_DLLITEM* right;
};

struct DLLHASHTABLE_HASHTABLEVALUE{
	void* value;
	struct DLLHASHTABLE_DLLITEM* dllitem_ptr;
	DLLHASHTABLE_INNER_DELVALUEFUNC delvaluefunc;
};

struct DLLHASHTABLE* dllhashtable_create(
	unsigned int nslots,
	HASHTABLE_COMPFUNC compfunc,
	HASHTABLE_HASHFUNC hashfunc,
	HASHTABLE_DELVALUEFUNC hashtable_delvaluefunc);

// void dllhashtable_destroy();

void dllhashtable_insert(struct DLLHASHTABLE* dllhashtable_ptr, 
	void*key, void* value);

struct DLLHASHTABLE_HASHTABLEVALUE* dllhashtable_search(
	struct DLLHASHTABLE* dllhashtable_ptr,
	void* key);

void dllhashtable_delete(struct DLLHASHTABLE* dllhashtable_ptr,
	void* key);

struct DLLHASHTABLE_HASHTABLEVALUE* dllhashtable_next(
	struct DLLHASHTABLE* dllhashtable_ptr,
	struct DLLHASHTABLE_HASHTABLEVALUE* hashtablevalue_ptr);

#endif

