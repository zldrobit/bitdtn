#ifndef _BPD_BIND_LIST_H_
#define _BPD_BIND_LIST_H_

#include "bp.h"
#include "uri.h"

#define BIND_LIST_MAXSIZE	100

extern struct BPD_BIND_LIST bpd_bind_list;

struct BPD_BIND_LIST{
	int nr;
	struct BIND_STRUCT{
		int valid;
		int local;
		struct URI bp_endpoint_id;
		struct sockaddr_un uaddr;
		struct sockaddr_in iaddr;
	}bind_structs[BIND_LIST_MAXSIZE];
};

void bpd_bind_list_init();
void bpd_bind_list_insert(struct URI* bp_endpoint_id_ptr, 
	struct sockaddr_un* uaddr_ptr, struct sockaddr_in* iaddr_ptr, 
	int local);
int bpd_bind_list_find_by_bp_endpoint_id(struct URI* bp_endpoint_id_ptr);
int bpd_bind_list_is_bp_endpoint_id_local(struct URI* bp_endpoint_id_ptr);
struct sockaddr_un* bpd_bind_list_map_bp_endpoint_id_to_uaddr(
	struct URI* bp_endpoint_id_ptr);
int bpd_bind_list_find_by_uaddr(struct sockaddr_un* uaddr_ptr);
struct URI* bpd_bind_list_map_uaddr_to_bp_endpoint_id(
	struct sockaddr_un* uaddr_ptr);
void bpd_bind_list_delete_by_uaddr(struct sockaddr_un* uaddr_ptr);
void bpd_bind_list_print();

#endif

