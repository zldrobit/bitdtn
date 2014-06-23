#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "bp.h"
#include "bundle.h"
#include "bpd_bundle_list.h"
#include "bpd_forward_table.h"
#include "dllhashtable.h"

pthread_mutex_t bpd_bundle_list_mutex = PTHREAD_MUTEX_INITIALIZER;
struct DLLHASHTABLE* dllhashtable_ptr;

static void bpd_bundle_list_send_custody_transfer_succeeded_signal(
	struct BUNDLE* bundle_ptr);

unsigned int hashfunc(struct HASHTABLE* hashtable_ptr, void* key)
{
	unsigned int hash;
	int nbits;
	unsigned int nslots;

	hash = fnv_32a_buf(key, 8, FNV1_32A_INIT);
	nslots = hashtable_ptr->nslots;
	if (hashtable_is2power(nslots)){
		nbits = hashtable_log2(nslots);
		hash = (hash >> nbits) & (hash & ((1 << nbits) - 1));
	}
	else {
		/* lazy mod without retry */
		hash %= nslots;
	}
	return hash;
}

int compfunc(char* l, char* r)
{
	// int i;
	// for (i = 0; i < 8; i++){
	// 	if (*l++ != *r++){
	// 		return 1;
	// 	}
	// }
	int a,b;
	a = *(unsigned*)l - *(unsigned*)r;
	b = *(unsigned*)(l+4) - *(unsigned*)(r+4);
	if (a == 0 && b == 0){
		return 0 ;
	}
	else {
		return 1;
	}
}



void bpd_bundle_list_init()
{
	pthread_t scan_thread;
	dllhashtable_ptr = dllhashtable_create(
		BPD_BUNDLE_LIST_MAXSLOTS,
		(HASHTABLE_COMPFUNC)compfunc,
		hashfunc,
		free);	
	pthread_create(&scan_thread, NULL, bpd_bundle_list_scan_thread, NULL);
}

void* bpd_bundle_list_scan_thread(void* arg)
{
	struct DLLHASHTABLE_HASHTABLEVALUE* hashtablevalue_ptr;
	struct BUNDLE* bundle_ptr;
	while(1){
		sleep(BPD_BUNDLE_LIST_SCAN_INTERVAL);
		pthread_mutex_lock(&bpd_bundle_list_mutex);
		hashtablevalue_ptr = dllhashtable_next(dllhashtable_ptr,
			NULL);
		while (hashtablevalue_ptr != NULL){
			bundle_ptr = hashtablevalue_ptr->value;
			bpd_forward(bundle_ptr);
			hashtablevalue_ptr = 
				dllhashtable_next(dllhashtable_ptr,
					hashtablevalue_ptr);
		}
		
		pthread_mutex_unlock(&bpd_bundle_list_mutex);
	}
}

void bpd_bundle_list_pop(struct BUNDLE* bundle_ptr)
{
	struct sockaddr_in remote_iaddr;
	
	// pthread_mutex_lock(&bpd_bundle_list_mutex);
	bpd_forward(bundle_ptr);
	// free(bundle_ptr);
	// pthread_mutex_unlock(&bpd_bundle_list_mutex);

}

void bpd_bundle_list_insert(struct BUNDLE* bundle_ptr)
{
	char* key;
	struct BUNDLE* new_bundle_ptr;
	// pthread_mutex_lock(&bpd_bundle_list_mutex);
	bundle_encode(bundle_ptr);
	// pthread_mutex_unlock(&bpd_bundle_list_mutex);

	if (!bundle_ptr->iscustody){
		bpd_bundle_list_pop(bundle_ptr);
	}
	else {
		/* src_bp_endpoint_id is not local */
		if (!bpd_bind_list_is_bp_endpoint_id_local(
			&bundle_ptr->src_bp_endpoint_id)){
			bpd_bundle_list_send_custody_transfer_succeeded_signal(
				bundle_ptr);
		}

		/* dst_bp_endpoint_id is local */
		if (bpd_bind_list_is_bp_endpoint_id_local(
			&bundle_ptr->dst_bp_endpoint_id)){
			bpd_bundle_list_pop(bundle_ptr);
		}
		else {
			new_bundle_ptr = 
				(struct BUNDLE*)malloc(sizeof(struct BUNDLE));
			memcpy(new_bundle_ptr, bundle_ptr, 
				sizeof(struct BUNDLE));

			uri_copy(&new_bundle_ptr->custodian_bp_endpoint_id, 
				&bpd_forward_table_custodian_bp_endpoint_id);
		
			key = (char*)malloc(
				sizeof(BPD_BUNDLE_LIST_HASHKEY_LEN));
			*(unsigned int*)key = bundle_ptr->creation_time;
			*(unsigned int*)(key+4) = 
				bundle_ptr->creation_sequence_number;
			pthread_mutex_lock(&bpd_bundle_list_mutex);
			dllhashtable_insert(dllhashtable_ptr, key, 
				new_bundle_ptr);
			pthread_mutex_unlock(&bpd_bundle_list_mutex);
		}
	}
}

void bpd_bundle_list_delete(unsigned int creation_time, 
	unsigned int creation_sequence_number)
{
	char* key;
	
	printf("bpd_bundle_list_delete()\n");
	printf("creation_time = %u\n", creation_time);
        printf("creation_sequence_number = %u\n", creation_sequence_number);

	key = (char*)malloc(BPD_BUNDLE_LIST_HASHKEY_LEN);
	*(unsigned int*)key = creation_time;
	*(unsigned int*)(key+4) = creation_sequence_number;
	pthread_mutex_lock(&bpd_bundle_list_mutex);
	dllhashtable_delete(dllhashtable_ptr, key);
	pthread_mutex_unlock(&bpd_bundle_list_mutex);
	free(key);
}

void bpd_bundle_list_send_custody_transfer_succeeded_signal(
	struct BUNDLE* bundle_ptr)
{
	struct CUSTODY_SIGNAL custody_signal;
	struct BUNDLE admin_record_bundle;
	unsigned int time_of_signal_sec;
	unsigned int time_of_signal_nsec;
	struct timespec ts;
	
	printf("bpd_bundle_list_send_custody_transfer_succeeded_signal()\n");
	
	admin_record_set_custody_signal((struct ADMIN_RECORD*)&custody_signal);
	admin_record_clr_flag((struct ADMIN_RECORD*)&custody_signal);
	custody_signal_set_succeeded_flag(&custody_signal);
	custody_signal_set_no_addition(&custody_signal);
	
	clock_gettime(CLOCK_REALTIME, &ts);
	time_of_signal_sec = (unsigned int)ts.tv_sec - SECS_EPOCH_TO_2000;
	time_of_signal_nsec = (unsigned int)ts.tv_nsec;
	custody_signal.time_of_signal_sec = time_of_signal_sec;
	custody_signal.time_of_signal_nsec = time_of_signal_nsec;
	custody_signal.creation_time = bundle_ptr->creation_time;
	custody_signal.creation_sequence_number = 
		bundle_ptr->creation_sequence_number;
	uri_copy(&custody_signal.src_endpoint_id, 
		&bundle_ptr->src_bp_endpoint_id);
	custody_signal_encode(&custody_signal);
	
	uri_copy(&admin_record_bundle.src_bp_endpoint_id,
		&bpd_forward_table_custodian_bp_endpoint_id);
	uri_copy(&admin_record_bundle.dst_bp_endpoint_id,
		&bundle_ptr->custodian_bp_endpoint_id);
	uri_assign(&admin_record_bundle.report_to_bp_endpoint_id,
		"", "");
	uri_assign(&admin_record_bundle.custodian_bp_endpoint_id,
		"", "");

	// not to explicit call bundle_clear_xxxx
	// bundle_set_xxxx out of the bundle_encode()
	// bundle_clear_bundle_proc_flags(&admin_record_bundle);
	// bundle_set_admin_record(&admin_record_bundle);
	admin_record_bundle.isadmin = 1;
	printf("signal after set admin_record:\n");
	bundle_print(&admin_record_bundle);

	admin_record_bundle.iscustody = 0;

	bundle_set_creation_time(
		&admin_record_bundle,
		time_of_signal_sec);

	pthread_mutex_lock(&bpd_creation_sequence_number_mutex);
	bundle_set_creation_sequence_number(
		&admin_record_bundle,
		bpd_creation_sequence_number_counter++);
	pthread_mutex_unlock(&bpd_creation_sequence_number_mutex);

	
	memcpy(admin_record_bundle.payload, custody_signal.payload,
		custody_signal.payload_len);
	admin_record_bundle.payload_block_length = custody_signal.payload_len;

	printf("signal before bundle encode:\n");
	bundle_print(&admin_record_bundle);
	bundle_encode(&admin_record_bundle);
	printf("signal before insert to bundle list:\n");
	bundle_print(&admin_record_bundle);
	bpd_bundle_list_insert(&admin_record_bundle);
}
