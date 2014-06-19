#ifndef _URI_H_
#define _URI_H_

#define URI_SCHEME_MAXLEN 1024
#define URI_SSP_MAXLEN 1024

struct URI{
	char scheme[URI_SCHEME_MAXLEN + 1];
	char ssp[URI_SSP_MAXLEN + 1];
};

void uri_init(struct URI* p);
void uri_assign(struct URI* p, char* scheme, char* ssp);
void uri_split(struct URI* p, char* scheme, char* ssp);
int uri_compare(struct URI* lhs, struct URI* rhs);
void uri_copy(struct URI* to, struct URI* from);

#endif

