#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "bp.h"
#include "bpd_bind_list.h"

struct BPD_BIND_LIST bpd_bind_list;

void bpd_bind_list_init()
{
	bpd_bind_list.nr = 0;	
}

void bpd_bind_list_insert(struct URI* bp_endpoint_id_ptr, 
	struct sockaddr_un* uaddr_ptr, struct sockaddr_in* iaddr_ptr, 
	int local)
{
	struct BIND_STRUCT* bind_struct_ptr;
	int bothnull = 1;

	printf("bpd_bind_list_insert upath %s\n", uaddr_ptr->sun_path);
	bind_struct_ptr = &bpd_bind_list.bind_structs[bpd_bind_list.nr];
	// strcpy(bind_struct_ptr->bp_endpoint_id, bp_endpoint_id);
	uri_copy(&bind_struct_ptr->bp_endpoint_id, bp_endpoint_id_ptr);
	if (uaddr_ptr != NULL){
		memcpy(&bind_struct_ptr->uaddr, 
			uaddr_ptr, sizeof(struct sockaddr_un));
		bothnull = 0;
	}
	if (iaddr_ptr != NULL){
		memcpy(&bind_struct_ptr->iaddr, 
			iaddr_ptr, sizeof(struct sockaddr_in));
		bothnull = 0;
	}
	// assert(bothnull == 0);
	if (bothnull == 1){
		printf("bpd_bind_list_insert uaddr and iaddr are null ptr!\n");
		exit(EXIT_FAILURE);
	}
	bind_struct_ptr->valid = 1;
	bind_struct_ptr->local = local;
	bpd_bind_list.nr++;
}

int bpd_bind_list_find_by_bp_endpoint_id(struct URI* bp_endpoint_id_ptr)
{
	struct BIND_STRUCT* bind_struct_ptr;
	int i;
	for (i = 0; i < bpd_bind_list.nr; i++){
		bind_struct_ptr = &bpd_bind_list.bind_structs[i];
		// if (bind_struct_ptr->valid == 0){
		// 	continue;
		// }
		if (uri_compare(bp_endpoint_id_ptr, 
			&bind_struct_ptr->bp_endpoint_id) == 0){
			return i;
		}
			
	}
	return -1;
}

int bpd_bind_list_is_bp_endpoint_id_local(struct URI* bp_endpoint_id_ptr)
{
	if (bpd_bind_list_find_by_bp_endpoint_id(bp_endpoint_id_ptr) == -1){
		return 0;
	}
	else {
		return 1;
	}
}

struct sockaddr_un* bpd_bind_list_map_bp_endpoint_id_to_uaddr(
	struct URI* bp_endpoint_id_ptr)
{
	int i;
	i = bpd_bind_list_find_by_bp_endpoint_id(bp_endpoint_id_ptr);
	if (i == -1){
		return NULL;
	} 
	else {
		return &bpd_bind_list.bind_structs[i].uaddr;
	}
}

int bpd_bind_list_find_by_uaddr(struct sockaddr_un* uaddr_ptr)
{
	int i;
	struct BIND_STRUCT* bind_struct_ptr;
	for (i = 0; i < bpd_bind_list.nr; i++){
		bind_struct_ptr = &bpd_bind_list.bind_structs[i];
		// if (bind_struct_ptr->valid == 0){
		// 	continue;
		// }
		if (strcmp(uaddr_ptr->sun_path, 
			bind_struct_ptr->uaddr.sun_path) == 0){
			return i;
		}
	}
	return -1;
}

struct URI* bpd_bind_list_map_uaddr_to_bp_endpoint_id(
	struct sockaddr_un* uaddr_ptr)
{
	int i;
	i = bpd_bind_list_find_by_uaddr(uaddr_ptr);
	if (i == -1){
		return NULL;
	} 
	else {
		return &bpd_bind_list.bind_structs[i].bp_endpoint_id;
	}
}

void bpd_bind_list_delete_by_uaddr(struct sockaddr_un* uaddr_ptr)
{
	int i;
	i = bpd_bind_list_find_by_uaddr(uaddr_ptr);
	if (i == -1){
		return ;
	}
	else {
		bpd_bind_list.bind_structs[i].valid = 0;
	}
}

void bpd_bind_list_print()
{
	int i;
	struct BIND_STRUCT* bind_struct_ptr;
	for (i = 0; i < bpd_bind_list.nr; i++){
		printf("bind_structs[%d]:\n", i);
		bind_struct_ptr = &bpd_bind_list.bind_structs[i];
		printf("valid = %d\n", bind_struct_ptr->valid);
		printf("local = %d\n", bind_struct_ptr->local);
		printf("bp_endpoint_id scheme = %s\n",
			bind_struct_ptr->bp_endpoint_id.scheme);
		printf("bp_endpoint_id ssp = %s\n",
			bind_struct_ptr->bp_endpoint_id.ssp);
		if (bind_struct_ptr->local == 1){
			printf("uaddr.sun_path = %s\n",
				bind_struct_ptr->uaddr.sun_path);
		}
 		else {
			printf("iaddr.sin_port = %d\n",
				htons(bind_struct_ptr->iaddr.sin_port));
			printf("iaddr.sin_addr = %d\n",
				inet_ntoa(bind_struct_ptr->iaddr.sin_addr));
		}
	}
}


