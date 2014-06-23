#ifndef _BPD_FORWARD_TABLE_H_
#define _BPD_FORWARD_TABLE_H_

#include "bp.h"
#include "uri.h"

#define FORWARD_TABLE_MAXSIZE		1000
#define FORWARD_TABLE_CONF_FILE_PATH	"etc/bitdtn/forward_table"
#define FORWARD_TABLE_CONF_LINE_MAXLEN	2096

extern struct URI bpd_forward_table_self_bp_endpoint_id;

struct BPD_FORWARD_TABLE{
	int nr;
	struct FORWARD_STRUCT{
		int valid;
		struct URI origin_bp_endpoint_id;
		struct sockaddr_in origin_iaddr;
		struct URI next_bp_endpoint_id;
		struct sockaddr_in next_iaddr;
	}forward_structs[FORWARD_TABLE_MAXSIZE]; 
};

void bpd_forward_table_init();
void bpd_forward_table_parse_conf();
void bpd_forward_table_print();
void bpd_forward_table_insert(struct URI* origin_bp_endpoint_id_ptr,
        struct sockaddr_in* origin_iaddr_ptr,
        struct URI* next_bp_endpoint_id_ptr,
        struct sockaddr_in* next_iaddr_ptr);
int bpd_forward_table_find_by_origin_bp_endpoint_id(
        struct URI* bp_endpoint_id_ptr);
int bpd_forward_table_find_by_origin_iaddr(struct sockaddr_in* iaddr_ptr);
void bpd_forward_table_delete_by_origin_bp_endpoint_id(
        struct URI* origin_bp_endpoint_id_ptr);
int bpd_forward(struct BUNDLE* bundle_ptr);
#endif

