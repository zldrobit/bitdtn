/*
 * cfdp.h
 *
 *  Created on: Apr 1, 2014
 *      Author: wenting
 */


#include <stdio.h>
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/un.h>
#include <sys/time.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include "../bp/bp.h"
#include "../bp/uri.h"

#ifndef CFDP_H_
#define CFDP_H_



#define a 3000
#define FILE_NAME_MAX_SIZE 8
#define Version 0
#define header_length 16
#define data_length 1000
#define NAK_number 1000


int TSN;
char buffer[3000000];
char CFDP_buffer_data[100000][data_length];
//char CFDP_buffer_NAK[1000];
int CFDP_buffer_NAK[100000];
int number_of_received_pacekt;
int metadata_flag;






enum TYPE {
	file_dir,
	file_data
};

enum DIRECTION {
	to_file_recv,
	to_file_send
};
enum Transmission_mode{
	acknowledged,
	unacknowledged
};
enum CRC_flag{
	not_present,
	present
};

struct NAK_flag{
	int flag;
	//char buffer[100];
	int buffer[NAK_number];
};

struct PDU_header{

	int version;
	enum TYPE type;
	enum DIRECTION dir;
	enum Transmission_mode mode;
	enum CRC_flag CRCflag;
	int reserved1;
	int PDUdatalength;
	int reserved2;
	int length_of_entity_ID;
	int reserved3;
	int length_of_TSN;
	int source_ID;
	int TSN;
	int destination_ID;

};

struct EOF_data{

	int directive_code  ;    //8bits
	int condition_code ;     ///4bits
	int spare ;              //4bits
	int file_checksum  ;     ///32bits
	int file_size;           ///32bits
	//////fault location is not used :omitted



};


struct ACK_data{

	int directive_code  ;          //8bits
	int directive_code2  ;         ///4bits
	int directive_subtype_code  ; ///4bits
	int condition_code ;          ///4bits
	int spare ;                   ///2bits
	int transaction_status ;      ///2bits


};

struct meta_data{
//////////////the name length is 3 for now////////////////////////
	int directive_code  ;         ///8bits
	int segmentation_control;     ///1bit
	int reserved;                 ///7bits
	int file_size;               ///32bits
	int source_file_name_length;  ///8bits
	unsigned char source_file_name[24];
	int destination_file_name_length;  ///8bits
	unsigned char destination_file_name[24];


};


struct NAK{

	int directive_code;  //8bits
	int start_of_scope;  //32bits
	int end_of_scope;    //32bits
	//char NAK_offset[100];  //100byte 
	int NAK_offset[NAK_number];  //400byte



};

struct NAK_arguments{

	struct meta_data meta;
	struct PDU_header *p;
	struct NAK_flag *nakflag;
	FILE* fp;

};

 void* NAK_thread(void *ptr);
 void* recv_thread();
 void* rate_thread();
void chars2bits(unsigned char chars[], unsigned char bits[], int nchar, int bits_offset);
void bits2chars(unsigned char bits[], unsigned char chars[], int nbit, int bits_offset);
int PDU_encode(struct PDU_header *p,unsigned char* CFDP_buffer_data,unsigned char* buffer,int file_size);
int PDU_discode(struct PDU_header *p,unsigned char* CFDP_buffer_data,unsigned char* buffer,int recv_size);
int metadata(struct meta_data meta,unsigned char *buffer_meta,struct PDU_header *p);
int EOF_PDU(struct EOF_data eof,unsigned char *buffer_EOF,struct PDU_header *p );
int ACK_PDU(struct ACK_data ack,unsigned char *buffer_ACK,struct PDU_header *p );
int NAK_PDU(struct NAK nak,unsigned char *CFDP_buffer_NAK,struct PDU_header *p );
int bit_exchange(int k,int b,unsigned char *buffer);
int char_exchange(int k,unsigned char*buffer);

int write_file(struct meta_data meta,FILE *fp);
int check_missing(struct meta_data meta,struct NAK_flag *nakflag);
int send_PDU(char *buffer,int PDU_size);
int sendfile(char *sourcefile_name,char *buffer,int file_size);
int cfdp_open(char *sourcefile_name,char *destfile_name,char *buffer);
int cfdp_put_request(int DestID,char *sourcefile_name,char *Destfile_name);
int metadata_send(char *sourcefile_name,char *destfile_name,int file_size);
int segment_procedure(int file_size,struct PDU_header *p, char *buffer);







#endif /* CFDP_H_ */
