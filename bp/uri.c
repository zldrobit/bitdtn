#include <string.h>
// #include <assert.h>
#include "uri.h"

void uri_init(struct URI* p)
{
	memset(p, 0, URI_SCHEME_MAXLEN+1);
	memset(p, 0, URI_SSP_MAXLEN+1);
}

void uri_assign(struct URI* p, char* scheme, char* ssp)
{
	strcpy(p->scheme, scheme);
	strcpy(p->ssp, ssp);
}

void uri_split(struct URI* p, char* scheme, char* ssp)
{
	strcpy(scheme, p->scheme);
	strcpy(ssp, p->ssp);
}

int uri_compare(struct URI* lhs, struct URI* rhs)
{
	int ret;
	ret = strcmp(lhs->scheme, rhs->scheme);
	if (ret != 0){
		return ret;
	}
	return strcmp(lhs->ssp, rhs->ssp);
}

void uri_copy(struct URI* to, struct URI* from)
{
	// assert(from->scheme != NULL);
	// assert(from->ssp != NULL);
	strcpy(to->scheme, from->scheme);
	strcpy(to->ssp, from->ssp);
}

