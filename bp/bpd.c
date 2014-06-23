#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <time.h>

#include "bp.h"
#include "bundle.h"
#include "bpd_bind_list.h"
#include "bpd_bundle_list.h"
#include "bpd_forward_table.h"

#define BUFFER_SIZE		5000
#define BUNDLE_LIST_MAXSIZE 	200

int bpd_usock, bpd_isock;
struct sockaddr_un bpd_uaddr;
struct sockaddr_in bpd_iaddr;
pthread_mutex_t bpd_creation_sequence_number_mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned int bpd_creation_sequence_number_counter = 0;

int dropped[101] = {0};

void bpd_process_recv_bundle(char* buffer)
{


}

void bpd_process_admin_record(struct BUNDLE* bundle_ptr)
{
	unsigned int creation_time;
	unsigned int creation_sequence_number;
	struct CUSTODY_SIGNAL custody_signal;


	creation_time = bundle_ptr->creation_time;
	creation_sequence_number = bundle_ptr->creation_sequence_number;
	
	printf("bpd_process_admin_record()\n");
	printf("creation_time = %u\n", creation_time);
	printf("creation_sequence_number = %u\n", creation_sequence_number);
	memcpy(custody_signal.payload, bundle_ptr->payload, 
		bundle_ptr->payload_block_length);
	custody_signal_decode(&custody_signal);
	if (custody_signal_is_succeeded_flag(&custody_signal)){
		bpd_bundle_list_delete(creation_time, 
			creation_sequence_number);
	}
}

void* bpd_isock_recv_thread(void* arg)
{
	struct sockaddr_in remote_iaddr;
	int remote_iaddr_len;
	char buffer[BUFFER_SIZE];
	struct BUNDLE bundle;
	int len;
	int isdrop;
	char* payload;

	while(1){
		remote_iaddr_len  = sizeof(remote_iaddr);
		len = recvfrom(bpd_isock, &buffer, sizeof(buffer), 0, 
			(struct sockaddr*) &remote_iaddr, 
			(socklen_t*) &remote_iaddr_len);
		printf("bpd_isock_recv_thread() recv returned!\n");
		if (len != -1){
			memcpy(bundle.bundle, buffer, len);
			bundle_decode(&bundle);
			bundle_print(&bundle);

			if (!bundle_is_admin_record(&bundle)){
				isdrop = 0;
				payload = bundle.payload;
				if (payload[0] == '1' && payload[1] == '\0'){
					/* drop once */
					if (!dropped[1]){
						dropped[1] = 1;
						isdrop = 1;
					}
				}
				else if (payload[0] == '1' &&
					payload[1] == '0' &&
					payload[2] == '0'){
					/* drop once */
					if (!dropped[100]){
						dropped[100] = 1;
						isdrop = 1;
					}
				}
				if (!isdrop){
					bpd_bundle_list_insert(&bundle);
				}
			}
		}
		else {
			perror("bpd_isock_recv_thread recvfrom error");
		}
		
	}
}

void bpd_process_send_bundle(char* buffer)
{

}

void bpd_process_signal_bind(struct BPD_BIND* signal_bind_ptr,
	struct sockaddr_un* remote_uaddr_ptr)
{
	printf("bpd_process_signal_bind upath %s\n", 
		remote_uaddr_ptr->sun_path);
	bpd_bind_list_insert(&signal_bind_ptr->bp_endpoint_id, 
		remote_uaddr_ptr, NULL, 1);
}

void bpd_process_signal_close(struct BPD_CLOSE* signal_close_ptr,
	struct sockaddr_un* remote_uaddr_ptr)
{
	bpd_bind_list_delete_by_uaddr(remote_uaddr_ptr);
}

void bpd_process_signal_send(struct BPD_SEND* signal_send_ptr,
	struct sockaddr_un* remote_uaddr_ptr)
{
	struct URI* src_bp_endpoint_id_ptr;
	unsigned int time_current;
	unsigned int time_2000;
	struct tm tm_dtn_epoch;
	struct BUNDLE* bundle_ptr;
	struct URI report_to_bp_endpoint_id;
	struct URI custodian_bp_endpoint_id;
	
	// strcpy(src_bp_endpoint_id,
	// 	bpd_bind_list_map_uaddr_to_bp_endpoint_id(remote_uaddr_ptr));

	bundle_ptr = (struct BUNDLE*)&signal_send_ptr->version;
	
	src_bp_endpoint_id_ptr = 
		bpd_bind_list_map_uaddr_to_bp_endpoint_id(remote_uaddr_ptr);
	if (src_bp_endpoint_id_ptr == NULL){
		printf("bpd_process can't map remote_uaddr"
			" to bp endpoint id\n");
	}
	// strcpy(signal_send_ptr->src_bp_endpoint_id,
	// 	src_bp_endpoint_id);
	uri_copy(&signal_send_ptr->src_bp_endpoint_id,
		src_bp_endpoint_id_ptr);

	uri_assign(&report_to_bp_endpoint_id, "", "");
	uri_copy(&signal_send_ptr->report_to_bp_endpoint_id,
		&report_to_bp_endpoint_id);

	uri_assign(&custodian_bp_endpoint_id, "", "");
	if (signal_send_ptr->iscustody){
		uri_copy(&custodian_bp_endpoint_id, 
			&bpd_forward_table_custodian_bp_endpoint_id);
	}
	uri_copy(&signal_send_ptr->custodian_bp_endpoint_id,
		&custodian_bp_endpoint_id);

	
	// bundle_clear_bundle_proc_flags(
	// 	(struct BUNDLE*) &signal_send_ptr->version);

	// tm_dtn_epoch.tm_sec = 0;
	// tm_dtn_epoch.tm_min = 0;
	// tm_dtn_epoch.tm_hour = 0;
	// tm_dtn_epoch.tm_mday = 1;
	// tm_dtn_epoch.tm_mon = 0;
	// tm_dtn_epoch.tm_year = 100;
	// tm_dtn_epoch.tm_isdst = 0;	
	// time_dtn_epoch = (unsigned int) (mktime(&tm_dtn_epoch) - timezone);
	time_current = (unsigned int) time(NULL);

	bundle_set_creation_time(
		(struct BUNDLE*) &signal_send_ptr->version,
		time_current - SECS_EPOCH_TO_2000);

	pthread_mutex_lock(&bpd_creation_sequence_number_mutex);
	bundle_set_creation_sequence_number(
		(struct BUNDLE*) &signal_send_ptr->version,
		bpd_creation_sequence_number_counter++);
	pthread_mutex_unlock(&bpd_creation_sequence_number_mutex);

	if (bundle_ptr->iscustody){
		uri_copy(&bundle_ptr->custodian_bp_endpoint_id,
			src_bp_endpoint_id_ptr);
	}
	bpd_bundle_list_insert(
		(struct BUNDLE*) &signal_send_ptr->version);

}

void* bpd_usock_recv_thread(void* arg)
{
	struct sockaddr_un remote_uaddr;
	int remote_uaddr_len;
	char buffer[BUFFER_SIZE];
	struct BPD_SIGNAL signal;
	int len;

	while(1){
		remote_uaddr_len = sizeof(remote_uaddr);
		len = recvfrom(bpd_usock, &signal, sizeof(signal), 0,
			(struct sockaddr*) &remote_uaddr,
			(socklen_t*) &remote_uaddr_len);
		if (len == -1){
			perror("bpd_usock_recv_thread recvfrom error");	
		}
		printf("bpd_usock_recv_thread recvfrom upath %s\n",
			remote_uaddr.sun_path);
		switch (signal.signal_type){
			case BPD_SIGNAL_BIND:
				printf("BPD_SIGNAL_BIND\n");
				bpd_process_signal_bind(
					(struct BPD_BIND*) &signal,
					&remote_uaddr);
				bpd_bind_list_print();
				break;
			case BPD_SIGNAL_CLOSE:
				printf("BPD_SIGNAL_CLOSE\n");
				bpd_process_signal_close(
					(struct BPD_CLOSE*) &signal,
					&remote_uaddr);
				break;
			case BPD_SIGNAL_SEND:
				printf("BPD_SIGNAL_SEND\n");
				bpd_process_signal_send(
					(struct BPD_SEND*) &signal,
					&remote_uaddr);
				break;
			case BPD_SIGNAL_RECV:
				break;
		}
	}
}

void testsdnv()
{
        unsigned int value = 0x7F;
        unsigned int s;
        unsigned int value2 = 0;
	unsigned int i;


	value = 123;
	sdnv_encode(&s, &value, 4);
	printf("123 encode as s = 0x%4X\n", s);

	value = 456;
	sdnv_encode(&s, &value, 4);
	printf("456 encode as s = 0x%4X\n", s);

	value = 789;
	sdnv_encode(&s, &value, 4);
	printf("789 encode as s = 0x%4X\n", s);

	for (i = 0; i <= 0xFF; i++){
		value2 = 0;
		value = i;
		sdnv_encode(&s, &value, 1);
		sdnv_decode(&value2, &s, NULL);
		if (value != value2){
			printf("error at value = 0x%4X value2 = 0x%4X",
				value, value2);
		}
	}
	printf("length = 1 passed!\n");
	for (i = 0x100; i <= 0xFFFF; i++){
		value2 = 0;
		value = i;
		sdnv_encode(&s, &value, 2);
		sdnv_decode(&value2, &s, NULL);
		if (value != value2){
			printf("error at value = 0x%4X value2 = 0x%4X",
				value, value2);
		}
	}
	printf("length = 2 passed!\n");
	for (i = 0x10000; i <= 0xFFFFFF; i++){
		value2 = 0;
		value = i;
		sdnv_encode(&s, &value, 3);
		sdnv_decode(&value2, &s, NULL);
		if (value != value2){
			printf("error at value = 0x%4X value2 = 0x%4X",
				value, value2);
		}
	}
	printf("length = 3 passed!\n");

	for (i = 0x1000000; i < 0xFFFFFFFF; i++){
		if (i % 0x10000000 == 0){
			printf("i = 0x%X\n", i);
		}
		value2 = 0;
		value = i;
		sdnv_encode(&s, &value, 4);
		sdnv_decode(&value2, &s, NULL);
		if (value != value2){
			printf("error at value = 0x%4X value2 = 0x%4X",
				value, value2);
		}
	}
	value2 = 0;
	value = 0xFFFFFFFF;
	sdnv_encode(&s, &value, 4);
	sdnv_decode(&value2, &s, NULL);
	if (value != value2){
		printf("error at value = 0x%4X value2 = 0x%4X",
			value, value2);
	}
	printf("length = 4 passed!\n");
        // printf("s = 0x%4X\n", s);
        // printf("value = 0x%4X value2 = 0x%4X\n", value, value2);

        exit(0);
}

void testbundle()
{
	struct BUNDLE a;
	a.version = 6;
	a.bundle_proc_flags = 0;
	uri_assign(&a.dst_bp_endpoint_id, "dtn", "1");
	uri_assign(&a.src_bp_endpoint_id, "xxx", "2");
	uri_assign(&a.report_to_bp_endpoint_id, "ipn", "3");
	uri_assign(&a.custodian_bp_endpoint_id, "self", "4");
	a.creation_time = 123;
	a.creation_sequence_number = 456;
	a.lifetime = 789;
	a.block_type = 1;
	a.block_proc_flags = 0;
	a.payload_block_length = 3;
	strcpy(a.payload, "abc");

	bundle_print(&a);
	bundle_encode(&a);
	printf("after encode:\n\n");
	bundle_print(&a);
	
	printf("after decode:\n\n");
	bundle_decode(&a);
	bundle_print(&a);
	exit(0);
}


int main()
{
	pthread_t isock_recv_thread, usock_recv_thread;
	int ret;

	// testsdnv();
	// testbundle();	
	printf("\n********************\n");
	printf("bpd started!\n");
	printf("********************\n\n");

	bpd_bind_list_init();
	bpd_bundle_list_init();
	bpd_forward_table_init();
	bpd_forward_table_print();

	bpd_uaddr.sun_family = AF_UNIX;
	strcpy(bpd_uaddr.sun_path, BPD_USOCK_ADDR);
	bpd_usock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (bind(bpd_usock, (struct sockaddr*) &bpd_uaddr, 
		sizeof(struct sockaddr_un)) == -1){
		printf("can't bind bpd_usock!\n");
		exit(EXIT_FAILURE);
	}
	// unlink(BPD_USOCK_ADDR);
	
	bpd_isock = socket(AF_INET, SOCK_DGRAM, 0);
	bpd_iaddr.sin_family = AF_INET;
	bpd_iaddr.sin_port = htons(BUNDLE_PORT);
	bpd_iaddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(bpd_isock, (struct sockaddr*) &bpd_iaddr,
		sizeof(struct sockaddr_in)) == -1){
		printf("can't bind bpd_isock\n");
		exit(EXIT_FAILURE);
	}
	
	ret = pthread_create(&isock_recv_thread, NULL, 
		bpd_isock_recv_thread, NULL);
	if (ret != 0){
		printf("can't create bpd_isock_recv_thread!\n");
		exit(EXIT_FAILURE);
	}
	ret = pthread_create(&usock_recv_thread, NULL, 
		bpd_usock_recv_thread, NULL);
	if (ret != 0){
		printf("can't create bpd_usock_recv_thread!\n");
		exit(EXIT_FAILURE);
	}

	pthread_join(isock_recv_thread, NULL);
	pthread_join(usock_recv_thread, NULL);
	close(bpd_usock);
	close(bpd_isock);


	return 0;
}

