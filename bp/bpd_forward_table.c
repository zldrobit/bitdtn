#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "bp.h"
#include "bpd_bind_list.h"
#include "bpd_forward_table.h"
#include "bundle.h"
#include "uri.h"

struct BPD_FORWARD_TABLE bpd_forward_table;
struct URI bpd_forward_table_self_bp_endpoint_id;

void bpd_forward_table_init()
{
	bpd_forward_table.nr = 0;	
	bpd_forward_table_parse_conf();
}

void bpd_forward_table_parse_conf()
{
	FILE* f;
	char buf[FORWARD_TABLE_CONF_LINE_MAXLEN];
	char tok1[FORWARD_TABLE_CONF_LINE_MAXLEN];
	char tok2[FORWARD_TABLE_CONF_LINE_MAXLEN];
	char* p;
	struct URI origin_bp_endpoint_id;
	struct URI next_bp_endpoint_id;
	struct sockaddr_in origin_iaddr;
	struct sockaddr_in next_iaddr;
	int isorigin;
	int is_self_bp_endpoint_id_parsed;

	getcwd(buf, 256);
	printf("cwd = %s\n", buf);
	f = fopen(FORWARD_TABLE_CONF_FILE_PATH, "r");
	if (f == NULL){
		printf("can not open forward table file\n");
		exit(EXIT_FAILURE);
	}

	is_self_bp_endpoint_id_parsed = 0;
	isorigin = 1;

	while (1){
		fgets(buf, FORWARD_TABLE_CONF_LINE_MAXLEN, f);
		if (feof(f)){
			break;
		}
		
		printf("buf = %s\n", buf);
		// printf("buf len = %d\n", strlen(buf));
		
	
		buf[strlen(buf) - 1] = '\0';
		// printf("after buf len = %d\n", strlen(buf));

		if (!is_self_bp_endpoint_id_parsed){
			p = strtok(buf, ": 	");
			if (p == NULL){
				goto END_OF_WHILE;
			}
			strcpy(tok1, p);
			if (tok1[0] == '#'){
				goto END_OF_WHILE;	
			}
			p = strtok(NULL, ": 	");
			strcpy(tok2, p);
			uri_assign(&bpd_forward_table_self_bp_endpoint_id,
				tok1, tok2);	
			is_self_bp_endpoint_id_parsed = 1;
			goto END_OF_WHILE;
		}
		p = strtok(buf, " 	");
		if (p == NULL){
			goto END_OF_WHILE;
		}
		strcpy(tok1, p);
		if (tok1[0] == '#'){
			goto END_OF_WHILE;	
		}
		printf("tok1 = %s\n", tok1);
		strcpy(tok2, strtok(NULL, " 	"));
		printf("tok2 = %s\n", tok2);

		/* parse original uri and iaddr */
		// printf("tok1 = %s\n", tok1); 
		p = strtok(tok1, ":");
		// printf("p = %s\n", p);
		if (isorigin){
			strcpy(origin_bp_endpoint_id.scheme, p);
		}
		else {
			strcpy(next_bp_endpoint_id.scheme, p);
		}
		p = strtok(NULL, ":");
		// printf("p = %s\n", p);
		if (isorigin){
			strcpy(origin_bp_endpoint_id.ssp, p);
		}
		else {
			strcpy(next_bp_endpoint_id.ssp, p);
		}

		// printf("before strcpy(tok2 ... \n");

		/* parse original uri and iaddr */
		// printf("tok2 = %s\n", tok2);
		if (isorigin){
			origin_iaddr.sin_family = AF_INET;
			origin_iaddr.sin_port = htons(BUNDLE_PORT);
			origin_iaddr.sin_addr.s_addr = inet_addr(tok2);
		}
		else {
			next_iaddr.sin_family = AF_INET;
			next_iaddr.sin_port = htons(BUNDLE_PORT);
			next_iaddr.sin_addr.s_addr = inet_addr(tok2);
		}
		if (!isorigin){
			bpd_forward_table_insert(
				&origin_bp_endpoint_id, &origin_iaddr,
				&next_bp_endpoint_id, &next_iaddr);
		}
		isorigin = 1 - isorigin;

END_OF_WHILE:
		;
	}
	fclose(f);
	printf("end of parse forward table config file!\n");
}

void bpd_forward_table_print()
{
	int i;
	struct FORWARD_STRUCT tmp;

	printf("forward table self bp endpoint id = %s:%s\n",
		bpd_forward_table_self_bp_endpoint_id.scheme,
		bpd_forward_table_self_bp_endpoint_id.ssp);
	printf("forward table has %d items(s):\n\n", bpd_forward_table.nr); 
	for (i = 0; i < bpd_forward_table.nr; i++){
		printf("the %dth is:\n", i);
		tmp = bpd_forward_table.forward_structs[i];
		printf("valid = %d\n", tmp.valid);
		printf("origin bp endpoint id = %s:%s\n", 
			tmp.origin_bp_endpoint_id.scheme,
			tmp.origin_bp_endpoint_id.ssp);
		printf("origin iaddr = %s\n", 
			inet_ntoa(tmp.origin_iaddr.sin_addr));
		printf("next bp endpoint id = %s:%s\n", 
			tmp.next_bp_endpoint_id.scheme,
			tmp.next_bp_endpoint_id.ssp);
		printf("next iaddr = %s\n", 
			inet_ntoa(tmp.next_iaddr.sin_addr));
		printf("\n");
	}	
}

void bpd_forward_table_insert(struct URI* origin_bp_endpoint_id_ptr, 
	struct sockaddr_in* origin_iaddr_ptr,
	struct URI* next_bp_endpoint_id_ptr,
	struct sockaddr_in* next_iaddr_ptr)
{
	struct FORWARD_STRUCT* forward_struct_ptr;
	forward_struct_ptr = &bpd_forward_table.forward_structs[
		bpd_forward_table.nr]; 	
	// strcpy(forward_struct_ptr->origin_bp_endpoint_id, 
	// 	origin_bp_endpoint_id);
	uri_copy(&forward_struct_ptr->origin_bp_endpoint_id, 
		origin_bp_endpoint_id_ptr);
	memcpy(&forward_struct_ptr->origin_iaddr, origin_iaddr_ptr,
		sizeof(struct sockaddr_in));
	// strcpy(forward_struct_ptr->next_bp_endpoint_id, 
	// 	next_bp_endpoint_id);
	uri_copy(&forward_struct_ptr->next_bp_endpoint_id, 
		next_bp_endpoint_id_ptr);
	memcpy(&forward_struct_ptr->next_iaddr, next_iaddr_ptr,
		sizeof(struct sockaddr_in));
	forward_struct_ptr->valid = 1;
	bpd_forward_table.nr++;
}

int bpd_forward_table_find_by_origin_bp_endpoint_id(
	struct URI* bp_endpoint_id_ptr)
{
	int i;
	struct FORWARD_STRUCT* forward_struct_ptr;

	for (i = 0; i < bpd_forward_table.nr; i++){
		forward_struct_ptr = &bpd_forward_table.forward_structs[i];
		if (forward_struct_ptr->valid == 1 &&
			uri_compare(bp_endpoint_id_ptr, 
			&forward_struct_ptr->origin_bp_endpoint_id)
				== 0){
			return i;
		}
	}
	return -1;
}

int bpd_forward_table_find_by_origin_iaddr(struct sockaddr_in* iaddr_ptr)
{
	int i;
	struct FORWARD_STRUCT* forward_struct_ptr;

	for (i = 0; i < bpd_forward_table.nr; i++){
		forward_struct_ptr = &bpd_forward_table.forward_structs[i];
		if (forward_struct_ptr->valid == 1 && 
			forward_struct_ptr->origin_iaddr.sin_addr.s_addr
			== iaddr_ptr->sin_addr.s_addr){
				return i;
		}
	}
	return -1;
}

void bpd_forward_table_delete_by_origin_bp_endpoint_id(
	struct URI* origin_bp_endpoint_id_ptr)
{
	int i;
	i = bpd_forward_table_find_by_origin_bp_endpoint_id(
		origin_bp_endpoint_id_ptr);
	if (i != -1){
		bpd_forward_table.forward_structs[i].valid = 0;
	}
	else {
		printf("bpd_forward_table_delete_by_origin_bp_endpoint_id "
			"delete an invalid origin_bp_endpoint_id!\n");
	}
}

/* and dispatch */
int bpd_forward(struct BUNDLE* bundle_ptr)
{
	struct URI origin_bp_endpoint_id;
	int i;
	struct sockaddr_in remote_iaddr;
	struct sockaddr_un local_uaddr;

	printf("enter bpd_foward\n");
	origin_bp_endpoint_id = bundle_ptr->dst_bp_endpoint_id;

	// if dispatch
	i = bpd_bind_list_find_by_bp_endpoint_id(&origin_bp_endpoint_id);	
	if (i != -1){
		local_uaddr = bpd_bind_list.bind_structs[i].uaddr;
		bundle_print(bundle_ptr);
		sendto(bpd_usock, bundle_ptr->payload, 
			bundle_ptr->payload_block_length, 0, 
			(struct sockaddr*) &local_uaddr,
			sizeof(local_uaddr));
		return 0;
	}

	// forward
	i = bpd_forward_table_find_by_origin_bp_endpoint_id(
		&origin_bp_endpoint_id);
	if (i != -1){
		remote_iaddr = 
			bpd_forward_table.forward_structs[i].next_iaddr;
		sendto(bpd_isock, bundle_ptr->bundle, 
			bundle_ptr->bundle_len, 0,
			(struct sockaddr*) &remote_iaddr, 
			sizeof(remote_iaddr));
		// for assurance of the udp's correctness
		usleep(300);
		return 0;
	}
	return -1;	
}

