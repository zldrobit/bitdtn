#ifndef _DLL_H_
#define _DLL_H_

struct DLLITEM{
	void* value;
	struct DLLITEM* left;
	struct DLLITEM* right;
};

typedef void (*DLL_DELVALUEFUNC)(void* value);

struct DLL{
	struct DLLITEM* head;
	DLL_DELVALUEFUNC delvaluefunc;
};

struct DLL* dll_create(DLL_DELVALUEFUNC delvaluefunc);
// void dll_destroy();
int dll_isempty(struct DLL* dll_ptr);
struct DLLITEM* dll_insert(struct DLL* dll_ptr, void* value);
void dll_delete(struct DLL* dll_ptr, struct DLLITEM* item_ptr);
struct DLLITEM* dll_next(struct DLL* dll_ptr, struct DLLITEM* item_ptr);

#endif

