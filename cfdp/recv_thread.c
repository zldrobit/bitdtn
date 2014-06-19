/*
 * recv_thread.c
 *
 *  Created on: Apr 1, 2014
 *      Author: wenting
 */


#include <errno.h>
#include "cfdp.h"
#include "time.h"

pthread_t tid_rate_thread;
 void* recv_thread(){

	printf("listening......\n");
	//int recvsocket;
	//struct sockaddr_in recv_addr;
	//recvsocket = socket(AF_INET,SOCK_DGRAM,0);

	//bzero(&recv_addr,sizeof(recv_addr));
	//recv_addr.sin_family = AF_INET;
	//recv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//recv_addr.sin_port = htons(a);

	//if(bind(recvsocket,(struct sockaddr*)&recv_addr,sizeof(recv_addr))==-1){
		//printf("recv bind error\n");
		//exit(1);
	//}

	extern int bpsock;

 /*   struct URI src_bp_endpoint_id;
	bpsock = bp_socket();
	uri_assign(&src_bp_endpoint_id, "bitdtn", "A");
	bp_bind(bpsock, &src_bp_endpoint_id);*/

    int source_entity_ID;
    int TSN;
    int file_size;
    int segment_number = 0;
    unsigned char file_name[4];

    struct meta_data meta;
    int metadata_recv = 0;
    int NAK_flag;

    int test = 0;
	int EOF_flag = 0;
	number_of_received_pacekt = 0;
	int ACK_EOF_flag = 0;

    FILE *fp;

	while(1){

		unsigned char recvbuf[1024];
		bzero(recvbuf,1024);
		struct PDU_header *p;
		p = (struct PDU_header *)malloc(sizeof(struct PDU_header));
		int segment_flag = 0 ;
		int myerrno;
		int filelength;

		//int filelength = recvfrom(recvsocket,recvbuf,1024,0,NULL,NULL);
		printf("bpsock = %u\n", bpsock);
		filelength = bp_recv(bpsock, recvbuf, 1024);


		///////////////////after receiving the packets,started to count the speed ///////////////////

		if (filelength == -1){
			myerrno = errno;
			perror("bp_recv");
			printf("errno = %u\n", myerrno);
			exit(1);
		}
		printf("have recived the data %d\n",filelength);

		int recv_length;
		recv_length = filelength - 16;
		unsigned char recv[recv_length];
		bzero(recv,recv_length);

		if(PDU_discode(p,recv,recvbuf,recv_length)==0){
			printf("have discaped,%d\n",filelength);
		}

		if(p->type == file_dir)
		{
			if(recv[0] == 4){
				printf("this is the EOF PDU the directive code is %d\n",recv[0]);

				///////////send the ack of eof///////////////////
				struct ACK_data ack_eof = {6,4,0,0,0};
				struct PDU_header *p_ack_eof;
				struct NAK_flag *nakflag;
				struct NAK_arguments *nakargu;

				p_ack_eof = (struct PDU_header *)malloc(sizeof(struct PDU_header));
				nakargu = (struct NAK_arguments *)malloc(sizeof(struct NAK_arguments));
				nakflag = (struct NAK_flag *)malloc(sizeof(struct NAK_flag));
				nakflag->flag = 0;

				p_ack_eof->version = Version;
				p_ack_eof->type = file_dir;
				p_ack_eof->dir = to_file_recv;
				p_ack_eof->mode = acknowledged;
				p_ack_eof->CRCflag = not_present;
				p_ack_eof->reserved1 = 0;
				p_ack_eof->reserved2 = 0;
				p_ack_eof->reserved3 = 0;
				p_ack_eof->PDUdatalength = 2;
				p_ack_eof->length_of_entity_ID = 7;
				p_ack_eof->length_of_TSN = 32;
				p_ack_eof->source_ID = p->destination_ID;
				p_ack_eof->TSN = 1;
				p_ack_eof->destination_ID = p->source_ID;
				unsigned char buffer_ACK_EOF[18];
				if(ACK_PDU(ack_eof,buffer_ACK_EOF,p_ack_eof) == 0)
				{
					printf("have encaped ACK_EOF,%ld\n",sizeof(buffer_ACK_EOF));
				}
				send_PDU(buffer_ACK_EOF,sizeof(buffer_ACK_EOF));


            //////////////////////detect the lost segment//////////////////////////

				if(EOF_flag == 0)
				{
				check_missing(meta,nakflag);
				printf("the nakflag is %d\n",nakflag->flag);
				if(nakflag->flag ==  0)
				{
					write_file(meta,fp);
					printf("the file is recieved completed in the first time\n");
					pthread_cancel(tid_rate_thread);
				}
				else
				{

					pthread_t tid_NAK_thread;
					nakargu->meta = meta;
					nakargu-> p = p;
					nakargu->nakflag = nakflag;
					nakargu->fp = fp;
					printf("in the recv thread :the retrans number is %d,nakflag is %d\n",nakargu->nakflag->buffer[0],nakargu->nakflag->flag);

					pthread_create(&tid_NAK_thread, NULL, NAK_thread, nakargu);




				}
				}
				EOF_flag = 1;


			}
			if((recv[0]) == 6){
				if((recv[1]>>4) == 4){   ////////////the ack of EOF//////////////

					if(ACK_EOF_flag == 0)
					{
						sleep(4);
						printf("have recieved the ACK EOF\n");
						pthread_mutex_lock(&mtx);
						pthread_cond_signal(&cond);
						pthread_mutex_unlock(&mtx);
						ACK_EOF_flag ++;					
					}
				}
			}

			if((recv[0]) == 7){
				/////////////////the metadata///////////////
				
				pthread_create(&tid_rate_thread, NULL, rate_thread, NULL);
				printf("**********has start the counting thread***********\n");

	             unsigned char buffer_filesize[32];
	        	bits2chars(recv+2,buffer_filesize,32,0);
	        	meta.file_size = char_exchange(32,buffer_filesize);
	        	meta.segmentation_control = (recv[1] >> 7);
	        	segment_flag = meta.segmentation_control;
	        	printf("the segment flag is  %d\n",meta.segmentation_control);
	        	if(meta.segmentation_control == 0)
	        	{
	        		segment_number = meta.file_size/data_length;
	        	}
	        	//char CFDP_buffer_data[meta.file_size];
			    meta.destination_file_name_length = recv[6];
			    //printf("the file name size is %d\n",file_name_size);

			    bzero(meta.destination_file_name,meta.destination_file_name_length+1);

			    memcpy(meta.destination_file_name,recv+11,meta.destination_file_name_length);
                printf("the recieved file name in the metadata is %s,length is %ld\n",file_name,sizeof(recv));
    			 fp = fopen((const char*)meta.destination_file_name,"w");
    			if(NULL == fp ) {

    				printf("File:\t Can Not Open To Write\n");
    				exit(1);

    			}
    			else{

    				printf("can open to write\n");

    			}
                metadata_recv = 1;
                //TSN = p -> TSN;

			}
			if(recv[0] == 8)
				////////////////the NAK//////////////////////
			{
				int retrans_num;
				//char offset_NAK[100];
				unsigned char offset_retran[400];
				int offset_NAK[100];
				char buffer_retrans[header_length+data_length+4];
				char buffer_retrans_data[data_length];
				//memcpy(offset_NAK,recv+9,100);
				memcpy(offset_retran,recv+9,400);
				//retrans_num = offset_NAK[0];


				printf("the retrans number in nak is %d\n",offset_retran[3]);
				int j;
				for(j=0;j<4*offset_retran[3];j++)
				{
					printf("the recieved nak is %d\n",offset_retran[j+4]);
				}


				int i;
				for(i = 0;i<100;i++)
				{
					//offset_NAK[i] = (((int)offset_retran[4*i])&255)<<24+(((int)offset_retran[4*i+1])&255)<<16+(((int)offset_retran[4*i+2])&255)<<8+(((int)offset_retran[4*i+3])&255);
		
					offset_NAK[i] = offset_retran[4*i]*(2^24)+offset_retran[4*i+1]*(2^16)+offset_retran[4*i+2]*(256)+offset_retran[4*i+3];
				}
				retrans_num = offset_NAK[0];

				for(j=0;j<offset_NAK[0];j++)
				{
					printf("the recieved nak is %d\n",offset_NAK[j+1]);
				} 
				
                printf("have recieved the NAK ,need to be retransmitted  number is %d\n",retrans_num);

				struct PDU_header *p_retrans;
				p_retrans = (struct PDU_header *)malloc(sizeof(struct PDU_header));
				p_retrans->version = Version;
				p_retrans->type = file_data;
				p_retrans->dir = to_file_recv;
				p_retrans->mode = acknowledged;
				p_retrans->CRCflag = not_present;
				p_retrans->reserved1 = 0;
				p_retrans->reserved2 = 0;
				p_retrans->reserved3 = 0;
				p_retrans->PDUdatalength = 2;
				p_retrans->length_of_entity_ID = 7;
				p_retrans->length_of_TSN = 32;
				p_retrans->source_ID = p->destination_ID;
				p_retrans->TSN = 1;
				p_retrans->destination_ID = p->source_ID;


				
				for(i = 0;i < retrans_num;i++)
				{
					unsigned int re_offset;
					char buffer_offset[32];
					re_offset = offset_NAK[i+1];
					printf("the re_offset is %d\n",re_offset);
					if((re_offset == (meta.file_size/data_length)&&(meta.file_size%data_length !=0))||((re_offset == (meta.file_size/data_length - 1)&&(meta.file_size%data_length ==0))))
					{

						bit_exchange(32,data_length*re_offset,buffer_offset);
				    	chars2bits(buffer_offset,buffer_retrans_data,32,0);

						memcpy(buffer_retrans_data+4,buffer+re_offset*data_length,(meta.file_size-re_offset*data_length));
						printf("the last segment need to be retransmitted  offset is %d\n",re_offset);
						//printf("the retransmit data is %s\n",buffer_retrans_data);
				    	PDU_encode(p_retrans,buffer_retrans_data, buffer_retrans,(data_length+4));
				    	send_PDU(buffer_retrans,sizeof(buffer_retrans));
				    	printf("have send the retrans segment\n");

					}
					else
					{
				    	bit_exchange(32,data_length*re_offset,buffer_offset);
				    	chars2bits(buffer_offset,buffer_retrans_data,32,0);

						memcpy(buffer_retrans_data+4,buffer+re_offset*data_length,data_length);
						printf("need to be retransmitted  offset is %d\n",re_offset);
						//printf("the retransmit data is %s\n",buffer_retrans_data+4);
				    	PDU_encode(p_retrans,buffer_retrans_data, buffer_retrans,(data_length+4));
				    	send_PDU(buffer_retrans,sizeof(buffer_retrans));

					}


				}






			}
		}

		if(p->type == file_data)
		{

			number_of_received_pacekt = number_of_received_pacekt + 1;
			if(segment_flag == 0)
			{
				int offset;

				unsigned char buffer_offset[32];
				bits2chars(recv,buffer_offset,32,0);
				offset = char_exchange(32,buffer_offset);
				printf("the recived PDU is %ld,offset is %d\n",sizeof(recv),offset);
				if((offset+data_length)<meta.file_size)
				{
					if(test ==0 && offset == 0)
					{}
					else
					{
					memcpy(&CFDP_buffer_data[offset/data_length],recv+4,data_length);
					CFDP_buffer_NAK[offset/data_length] = 1		;
					printf("recvdata is put in buffer %d\n",offset/data_length);
					}
					//printf("recvdata is put in buffer %d\n",offset/data_length);
					//printf("%s\n",CFDP_buffer_data[offset/data_length]);

				}
				else
				{
					if(test != 0)
					{
						memcpy(&CFDP_buffer_data[offset/data_length],recv+4,(meta.file_size-offset));
						CFDP_buffer_NAK[offset/data_length] = 1;
						printf("the last recvdata is put in buffer %d\n",offset/data_length);
						//printf("%s\n",CFDP_buffer_data[offset/data_length]);
					}
					//printf("%s\n",CFDP_buffer_data[offset/data_length]);
					test = test +1;
					//printf("the number of test is %d\n",test);
				}


			}
			else if(segment_flag == 1)
			{

				memcpy(&CFDP_buffer_data[0],recv+4,meta.file_size);


			}




		}

 		
	}


}




