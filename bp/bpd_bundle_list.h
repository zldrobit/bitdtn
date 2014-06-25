#ifndef _BPD_BUNDLE_LIST_H_
#define _BPD_BUNDLE_LIST_H_

#include "bundle.h"
#include "dllhashtable.h"

#define BPD_BUNDLE_LIST_MAXSLOTS 			1024
// #define BPD_BUNDLE_LIST_MAXSIZE 			1024
#define BPD_BUNDLE_LIST_HASHKEY_LEN			8
#define BPD_BUNDLE_LIST_SCAN_INTERVAL			5
#define BPD_BUNDLE_LIST_REPLICA_LIST_MAXSLOTS		1024
#define BPD_BUNDLE_LIST_REPLICA_LIFETIME		60
#define BPD_BUNDLE_LIST_REPLICA_LIST_SCAN_INTERVAL	5

struct BPD_BUNDLE_LIST{
	int nr;
	
}bpd_bundle_list;

// extern struct DLLHASHTABLE* bpd_bundle_list_ptr;

void bpd_bundle_list_init();
void* bpd_bundle_list_scan_thread(void* arg);
void bpd_bundle_list_pop(struct BUNDLE* bundle_ptr);
void bpd_bundle_list_insert(struct BUNDLE* bundle_ptr);
void bpd_bundle_list_insert_custody(struct BUNDLE* bundle_ptr);
void bpd_bundle_list_delete(unsigned int creation_time, 
	unsigned int creation_sequence_number);

#endif

