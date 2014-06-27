/*
 * encape.c
 *
 *  Created on: Apr 1, 2014
 *      Author: wenting
 */

#include "cfdp.h"

void chars2bits(unsigned char chars[], unsigned char bits[], int nchar, int bits_offset)
{
	int i;
	int nbit = nchar/8 + 1;
	for (i = 0; i < nchar; i++){
		if (chars[i]){
			bits[(i+bits_offset)/8] |= 1 << (7 - ((i+bits_offset) % 8));
		}
		else {
			bits[(i+bits_offset)/8] &= ~(1 << (7 - ((i+bits_offset) % 8)));
		}
	}
}

void bits2chars(unsigned char bits[], unsigned char chars[], int nbit, int bits_offset)
{
	int i,j;
	int headlen;
	if (bits_offset % 8 == 0){
		for (i = 0; i < nbit/8; i++){
			for (j = 7; j >= 0; j--){
				chars[8*i+7-j] = (bits[i+bits_offset/8] & (1<<j)) >> j;
			}
		}
		for (j = 7; j > 7 - nbit % 8; j--){
			chars[8*i+7-j] = (bits[i+bits_offset/8] & (1<<j)) >> j;
		}
	}
	else {
		headlen = 8 - (bits_offset%8);
		for (j = headlen - 1; j >= 0; j--){
			chars[headlen - 1 - j] = (bits[bits_offset/8] & (1<<j)) >> j;
		}
		bits2chars(bits, chars + headlen, nbit - headlen, bits_offset + headlen);
	}
}

int PDU_encode(struct PDU_header *p,unsigned char* CFDP_buffer_data,unsigned char* buffer,int file_size)
{

	unsigned char buffer1[3];  //VERSION
	bzero(buffer1,3);
	unsigned char buffer2[1];  //pdu type
	unsigned char buffer3[1];  //direction
	unsigned char buffer4[1];  //transmission mode
	unsigned char buffer5[1];  //CRC flag
	unsigned char buffer6[1];  //reserved
	unsigned char buffer7[16]; //PDU data field length
	unsigned char buffer8[1];  //reserved
	unsigned char buffer9[3];  //length of entity IDS
	unsigned char buffer10[1]; //reserved
	unsigned char buffer11[3]; //length of transaction sequence number
	unsigned char buffer12[32]; //source entity ID
	bzero(buffer12,32);
	unsigned char buffer13[32]; //transaction sequence number
	bzero(buffer13,32);
	unsigned char buffer14[32]; //destination entity ID
	bzero(buffer14,32);
	bit_exchange(3,p->version,buffer1);
	chars2bits(buffer1,buffer,3,0);
	buffer2[0] = p->type;
	chars2bits(buffer2,buffer,1,3);
	buffer3[0] = p->dir;
	chars2bits(buffer3,buffer,1,4);
	buffer4[0] = p->mode;
	chars2bits(buffer4,buffer,1,5);
	buffer5[0] = p->CRCflag;
	chars2bits(buffer5,buffer,1,6);
	buffer6[0] = p->reserved1;
	chars2bits(buffer6,buffer,1,7);
	bit_exchange(16,p->PDUdatalength,buffer7);
	chars2bits(buffer7,buffer,16,8);
	buffer8[0] = p->reserved2;
	chars2bits(buffer8,buffer,1,24);
	bit_exchange(3,p->length_of_entity_ID,buffer9);
	chars2bits(buffer9,buffer,3,25);
	buffer10[0] = p->reserved3;
	chars2bits(buffer10,buffer,1,28);
	bit_exchange(3,p->length_of_TSN,buffer11);
	chars2bits(buffer11,buffer,3,29);
	bit_exchange(32,p->source_ID,buffer12);
	chars2bits(buffer12,buffer,32,32);
	bit_exchange(32,p->TSN,buffer13);
	chars2bits(buffer13,buffer,32,64);
	bit_exchange(32,p->destination_ID,buffer14);
	chars2bits(buffer14,buffer,32,96);

	memcpy(buffer+16,CFDP_buffer_data,file_size);
	return 0;
}

int PDU_discode(struct PDU_header *p,unsigned char* CFDP_buffer_data,unsigned char* buffer,int recv_size){

	unsigned char buffer1[3];
	bzero(buffer1,3);
	unsigned char buffer2[1];
	unsigned char buffer3[1];
	unsigned char buffer4[1];
	unsigned char buffer5[1];
	unsigned char buffer6[1];
	unsigned char buffer7[16];
	unsigned char buffer8[1];
	unsigned char buffer9[3];
	unsigned char buffer10[1];
	unsigned char buffer11[3];
	unsigned char buffer12[32];
	bzero(buffer12,32);
	unsigned char buffer13[32];
	bzero(buffer13,32);
	unsigned char buffer14[32];
	bzero(buffer14,32);



	bits2chars(buffer,buffer1,3,0);
	p->version = char_exchange(3,buffer1);
	bits2chars(buffer,buffer2,1,3);
	p->type = buffer2[0];
	bits2chars(buffer,buffer3,1,4);
	p->dir = buffer3[0];
	bits2chars(buffer,buffer4,1,5);
	p->mode = buffer4[0];
	bits2chars(buffer,buffer5,1,6);
	p->CRCflag = buffer2[0];
	bits2chars(buffer,buffer6,1,7);
	p->reserved1 = buffer2[0];
	bits2chars(buffer,buffer7,16,8);
	p->PDUdatalength = char_exchange(16,buffer7);

	bits2chars(buffer,buffer8,1,24);
	p->reserved2 = buffer8[0];
	bits2chars(buffer,buffer9,3,25);
	p->length_of_entity_ID = char_exchange(3,buffer9);
	//printf("the length of entity id is = %d\n",p->length_of_entity_ID);
	bits2chars(buffer,buffer10,1,28);
	p->reserved3 = buffer10[0];
	bits2chars(buffer,buffer11,3,29);
	p->length_of_TSN = char_exchange(3,buffer11);
	bits2chars(buffer,buffer12,32,32);
	p->source_ID = char_exchange(32,buffer12);
	bits2chars(buffer,buffer13,32,64);
	p->TSN = char_exchange(32,buffer13);
	bits2chars(buffer,buffer14,32,96);
	p->destination_ID = char_exchange(32,buffer14);
	//printf("the source and destination ID is %d,%d\n",p->source_ID,p->destination_ID);
	memcpy(CFDP_buffer_data,buffer+16,recv_size+1);



	return 0;
}

////////////just the name of the abc  for now/////////////////////
int metadata(struct meta_data meta,unsigned char *buffer_meta,struct PDU_header *p){
	/////////////////here we ommitted the options(including four parts)///////////////////
	unsigned char buffer0[8];
	unsigned char buffer1[1];
	unsigned char buffer2[7];
	unsigned char buffer3[32];
	unsigned char buffer4[8];
	unsigned char buffer5[24];
	bzero(buffer5,24);
	unsigned char buffer6[8];
	unsigned char buffer7[24];
	bzero(buffer7,24);

	unsigned char test[4];
	bzero(test,4);
	unsigned char test1[5];
	bzero(test1,5);


	unsigned char buffer_temp[14];
	bzero(buffer_temp,14);
	bit_exchange(8,meta.directive_code,buffer0);
	chars2bits(buffer0,buffer_temp,8,0);
	buffer1[0] = meta.segmentation_control;
	chars2bits(buffer1,buffer_temp,1,8);
	bit_exchange(7,meta.reserved,buffer2);
	chars2bits(buffer2,buffer_temp,7,9);
	bit_exchange(32,meta.file_size,buffer3);
	chars2bits(buffer3,buffer_temp,32,16);
	bit_exchange(8,meta.source_file_name_length,buffer4);
	chars2bits(buffer4,buffer_temp,8,48);
	//printf("the metadata source file name length is %d\n",buffer_temp[6]);

	memcpy(buffer_temp+7,meta.source_file_name,3);
	memcpy(test,buffer_temp+7,3);

	chars2bits(buffer5,buffer_temp,8,56);
	bit_exchange(8,meta.destination_file_name_length,buffer6);
	chars2bits(buffer6,buffer_temp,8,80);
	//bit_exchange(24,meta.destination_file_name,buffer7);
	//chars2bits(buffer7,buffer_temp,24,88);
	memcpy(buffer_temp+11,meta.destination_file_name,3);

	PDU_encode(p,buffer_temp, buffer_meta,14);


	return 0;
	;
}

int EOF_PDU(struct EOF_data eof,unsigned char *buffer_EOF,struct PDU_header *p )
{
	///////here the condition code is considered as no error,the fault location is omitted////////
	unsigned char buffer0[8];   //directive code octet
	unsigned char buffer1[4];   //condition code
	unsigned char buffer2[4];   //spare
	unsigned char buffer3[32];  //file checksum
	unsigned char buffer4[32];  //file size
	unsigned char buffer_temp[10];
	bit_exchange(8,eof.directive_code,buffer0);
	chars2bits(buffer0,buffer_temp,8,0);
	bit_exchange(4,eof.condition_code,buffer1);
	chars2bits(buffer1,buffer_temp,4,8);
	bit_exchange(4,eof.spare,buffer2);
	chars2bits(buffer2,buffer_temp,4,12);
	bit_exchange(32,eof.file_checksum,buffer3);
	chars2bits(buffer3,buffer_temp,32,16);
	bit_exchange(32,eof.file_size,buffer4);
	chars2bits(buffer4,buffer_temp,32,48);
	PDU_encode(p,buffer_temp, buffer_EOF,10);


	return 0;
}

int ACK_PDU(struct ACK_data ack,unsigned char *buffer_ACK,struct PDU_header *p )
{
	unsigned char buffer0[8];
	unsigned char buffer1[4];
	unsigned char buffer2[4];
	unsigned char buffer3[4];
	unsigned char buffer4[2];
	unsigned char buffer5[2];
	unsigned char buffer_temp[3];
	bit_exchange(8,ack.directive_code,buffer0);
	chars2bits(buffer0,buffer_temp,8,0);
	bit_exchange(4,ack.directive_code2,buffer1);
	chars2bits(buffer1,buffer_temp,4,8);
	bit_exchange(4,ack.directive_subtype_code,buffer2);
	chars2bits(buffer2,buffer_temp,4,12);
	bit_exchange(4,ack.condition_code,buffer3);
	chars2bits(buffer3,buffer_temp,4,16);
	bit_exchange(2,ack.spare,buffer4);
	chars2bits(buffer4,buffer_temp,2,20);
	bit_exchange(2,ack.transaction_status,buffer5);
	chars2bits(buffer5,buffer_temp,2,22);
	PDU_encode(p,buffer_temp, buffer_ACK,3);


	return 0;
}


int NAK_PDU(struct NAK nak,unsigned char *CFDP_buffer_NAK,struct PDU_header *p )
{
	unsigned char buffer0[8];
	unsigned char buffer1[32];
	unsigned char buffer2[32];
	//unsigned char buffer3[100];
	unsigned char buffer3[4*NAK_number];
	//unsigned char buffer_temp[109];
	unsigned char buffer_temp[4*NAK_number+9];
	bit_exchange(8,nak.directive_code,buffer0);
	chars2bits(buffer0,buffer_temp,8,0);
	bit_exchange(32,nak.start_of_scope,buffer1);
	chars2bits(buffer1,buffer_temp,32,8);
	bit_exchange(32,nak.end_of_scope,buffer2);
	chars2bits(buffer2,buffer_temp,32,40);
	//memcpy(buffer_temp+9,nak.NAK_offset,100);
	int i = 0;
	for(i = 0;i<NAK_number;i++)
	{
		buffer3[4*i+0] = nak.NAK_offset[i]>>24;
		buffer3[4*i+1] = nak.NAK_offset[i]>>16;
		buffer3[4*i+2] = nak.NAK_offset[i]>>8;
		buffer3[4*i+3] = nak.NAK_offset[i];
	}
	memcpy(buffer_temp+9,buffer3,4*NAK_number);
	//PDU_encode(p,buffer_temp, CFDP_buffer_NAK,109);
	PDU_encode(p,buffer_temp, CFDP_buffer_NAK,(4*NAK_number+9));

	return 0;
}



int bit_exchange(int k,int b,unsigned char *buffer){

	int i;
	for(i = 0;i<k;i++)
	{
		buffer[i] = (b&(1<<(k-1-i)))>>(k-1-i);
	}

	return 0;
}


int char_exchange(int k,unsigned char*buffer){

	int i;
	int b = 0;
	for(i = 0;i < k ; i ++){
		if(buffer[i])
		{
			b |= (1<<(k-1-i));
		}
	}


	return b;
}



