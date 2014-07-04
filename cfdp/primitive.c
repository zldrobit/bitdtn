/*
 * primitive.c
 *
 *  Created on: Apr 1, 2014
 *      Author: wenting
 */


#include "cfdp.h"
	 pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
	 pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	
	 pthread_mutex_t meta_mtx = PTHREAD_MUTEX_INITIALIZER;
	 pthread_cond_t meta_cond = PTHREAD_COND_INITIALIZER;

int write_file(struct meta_data meta,FILE *fp,struct recv_inf *recieve){

	printf("in the writing part...\n");


	int i;
	int write_return;
	int number;
	if(meta.segmentation_control == 0)
	{
	   if(meta.file_size%data_length == 0)
	   {
		   number = meta.file_size/data_length;

	   }
	   else
	   {
		   number = meta.file_size/data_length+1;
	   }
	for(i = 0;i<number;i++)
	{
		if((i == (meta.file_size/data_length)) && (meta.file_size/data_length != 0))
		{
			write_return = fwrite(recieve->CFDP_buffer_data[i],sizeof(char),(meta.file_size - (meta.file_size/data_length)*data_length),fp);
			//printf("writing.....i=%d,%s\n",i,recieve->CFDP_buffer_data[i]);
			printf("writing the last part of the file,the length is %d\n",write_return);
			perror("fwrite");
			fclose(fp);
			//exit(0);
		}
		else
		{

			write_return = fwrite(recieve->CFDP_buffer_data[i],sizeof(char),data_length,fp);
			//printf("writing.....i=%d,%s\n",i,recieve->CFDP_buffer_data[i]);
			perror("fwrite");
		}

	}

	}
	if(meta.segmentation_control == 1)
	{
		printf("in the writing part:the recieved file is %s\n",recieve->CFDP_buffer_data[0]);
		write_return = fwrite(recieve->CFDP_buffer_data[0],sizeof(char),meta.file_size,fp);

		/*if(ferror(fp))
		{
			perror("fwrite");		

			clearerr(fp);
		}

		printf("the return is %d\n",write_return);
		fclose(fp);*/
		
	}
	return 0;
}

int check_missing(struct meta_data meta,struct NAK_flag *nakflag){

	printf("checking the missing part....\n");
   int i,j;
   j = 0;
   int number;
   if(meta.file_size%data_length == 0)
   {
	number = meta.file_size/data_length;


   }
   else
   {
	   number = meta.file_size/data_length+1;
   }

	printf("the segment number is %d",number);	
   for(i = 0;i<number;i++)
   {
	   if(CFDP_buffer_NAK[i] == 1)
		   ///////have already recieved the PDU///////////
	   {

	   }
	   else
		   ///////need to be retransmitted////////////////
	   {
           nakflag->buffer[j+1] = i;
           nakflag->flag = 1;

        printf("the offset is missing %d,the retrans number is %d,nakflag is %d\n",nakflag->buffer[j+1],i,nakflag->flag);
        j = j + 1;
	   }
   }
    nakflag->buffer[0] = j;
    printf("the number of retranmission part is %d\n",j);


	return 0;
}



int sendfile(char *sourcefile_name,char *buffer,int file_size){

	unsigned char buffer_send[(data_length+16)];
	unsigned char buffer_EOF[26];
	struct PDU_header *p;
	p = (struct PDU_header *)malloc(sizeof(struct PDU_header));
	p->version = Version;
	p->type = file_data;
	p->dir = to_file_recv;
	p->mode = acknowledged;
	p->CRCflag = not_present;
	p->reserved1 = 0;
	p->reserved2 = 0;
	p->reserved3 = 0;
	p->PDUdatalength = data_length;
	p->length_of_entity_ID = 7;
	p->length_of_TSN = 32;
	p->source_ID = 1;
	p->TSN = 1;
	p->destination_ID = 2;


	struct PDU_header *p_eof;
	p_eof = (struct PDU_header *)malloc(sizeof(struct PDU_header));
	p_eof->version = Version;
	p_eof->type = file_dir;
	p_eof->dir = to_file_recv;
	p_eof->mode = acknowledged;
	p_eof->CRCflag = not_present;
	p_eof->reserved1 = 0;
	p_eof->reserved2 = 0;
	p_eof->reserved3 = 0;
	p_eof->PDUdatalength = data_length;
	p_eof->length_of_entity_ID = 7;
	p_eof->length_of_TSN = 32;
	p_eof->source_ID = 1;
	p_eof->TSN = 1;
	p_eof->destination_ID = 2;


    if(file_size > data_length)
    {
        printf("having segment\n");
    	segment_procedure(file_size,p,buffer);

    }
    else
    {
    	if(PDU_encode(p,buffer,buffer_send,file_size)==0)
    	{
    		//printf("have encaped,%d\n",sizeof(buffer_send));
    	}
    	else
    		printf("the encode have failed\n");
    	send_PDU(buffer_send,(file_size+16));
    }


	char EOF_buffer[1] = {0};

	struct EOF_data eof = {4,0,0,file_size};
	if(EOF_PDU(eof,buffer_EOF,p_eof)==0){
		printf("have encaped EOF,%ld\n",sizeof(buffer_EOF));
	}


	struct timespec ts;
	int ret;
	////////////////////////////sending EOF and turn on the EOF timer(here we set the timer 3s)//////////////////////

	while(1){

		send_PDU(buffer_EOF,sizeof(buffer_EOF));
		clock_gettime(CLOCK_REALTIME,&ts);   //获取时间
		ts.tv_sec += 3;   //经过3秒钟
		printf(" waiting for the ACK of EOF\n");
		ret = pthread_cond_timedwait(&cond,&mtx,&ts);
		if (ret == ETIMEDOUT){  //超时

			printf("EOF ETIMEDOUT\n");
				//break;

		}

		else {    //未超时  有中断

			printf("INTERRUPT\n");
			pthread_cond_destroy(&cond);

			break;


		}



	}


	printf("have recived the ACK EOF\n");
	return 0;

}
int cfdp_open(char *sourcefile_name,char *destfile_name,char *buffer){



	int file_size;
	int open_buffer_size;
	int i;

	FILE *fp = fopen((const char*)sourcefile_name,"r");

	if(NULL == fp )
	{
		printf("File:\t%s Not Found\n", sourcefile_name);
	}

	//get the length of the file
	struct stat fileData;     //stat:get the information of file
	if (0 == stat(sourcefile_name, &fileData))
	{
		file_size = fileData.st_size;
		printf("file size %d.\n", (unsigned int)fileData.st_size);

	}
	open_buffer_size = fread(buffer,sizeof(char),file_size,fp);
    printf("the file_send buffer is %d\n",open_buffer_size);


	//send the metadata
	metadata_send(sourcefile_name,destfile_name,file_size);
	printf("have send the metadata\n");
	sleep(2);



	//send the file

	sendfile(sourcefile_name,buffer,file_size);
    fclose(fp);
	return 0;
}


int cfdp_put_request(int DestID,char *sourcefile_name,char *Destfile_name){
	//char buffer[1024];
	bzero(buffer,1024);
	cfdp_open(sourcefile_name,Destfile_name,buffer);

	return 0;

}

int metadata_send(char *sourcefile_name,char *destfile_name,int file_size){

	unsigned char buffer_meta[30];
	struct PDU_header *p_meta ;
	p_meta = (struct PDU_header *)malloc(sizeof(struct PDU_header));
	p_meta->version = Version;
	p_meta->type = file_dir;
	p_meta->dir = to_file_recv;
	p_meta->mode = acknowledged;
	p_meta->CRCflag = not_present;
	p_meta->reserved1 = 0;
	p_meta->reserved2 = 0;
	p_meta->reserved3 = 0;
	p_meta->PDUdatalength = 14;
	p_meta->length_of_entity_ID = 7;
	p_meta->length_of_TSN = 32;
	p_meta->source_ID = 1;
	p_meta->TSN = 1;
	p_meta->destination_ID = 2;

	struct meta_data meta;
	if(file_size > data_length)
	{
	    meta.segmentation_control = 0;
	}
	else
		meta.segmentation_control = 1;


	meta.directive_code = 7;
    meta.reserved = 0;
    meta.file_size = file_size;
    meta.source_file_name_length = strlen(sourcefile_name);
    //printf("the meta.source_file_name_length is %d\n",meta.source_file_name_length);
    memcpy(meta.source_file_name , sourcefile_name,FILE_NAME_MAX_SIZE);

    meta.destination_file_name_length = strlen(destfile_name);
    memcpy(meta.destination_file_name , destfile_name,sizeof(destfile_name));
    //printf("the send metadata destfile name is %s\n",meta.destination_file_name);

    metadata(meta,buffer_meta,p_meta);
 if(workmode == 0)
{
	send_PDU(buffer_meta,30);
}

if(workmode == 1)
{
	struct timespec ts;
	int ret;

	while(1){

		    send_PDU(buffer_meta,30);
		clock_gettime(CLOCK_REALTIME,&ts);   //获取时间
		ts.tv_sec += 3;   //经过3秒钟
		printf(" waiting for the ACK of metadata\n");
		ret = pthread_cond_timedwait(&meta_cond,&meta_mtx,&ts);
		if (ret == ETIMEDOUT){  //超时

			printf("metadata ETIMEDOUT\n");
				//break;

		}

		else {    //未超时  有中断

			printf("INTERRUPT\n");
			pthread_cond_destroy(&meta_cond);

			break;


		}
		}

}






	return 0;
}

int segment_procedure(int file_size,struct PDU_header *p, char *buffer){

    int n;
    int i;
    int offset = 0;
    unsigned char buffer_recvdata[data_length+4];
    unsigned char buffer_send[header_length+4+data_length];
    unsigned char buffer_offset[32];
    printf("the file_size in the segment_procedure is %d\n",file_size);


    for(n = 0;n < file_size;(n = n+data_length))
    {

    	//////////encape the offset/////////////////////////
    	bzero(buffer_recvdata,data_length+4);
    	bzero(buffer_send,(header_length+data_length+4));
    	bzero(buffer_offset,32);
    	bit_exchange(32,offset,buffer_offset);
    	chars2bits(buffer_offset,buffer_recvdata,32,0);
    	////////////////////////////////////////////////////
    	if((file_size >(offset+data_length)) )
    	{
    		memcpy(buffer_recvdata+4,buffer+offset,data_length);

    	}
    	else
    	{
    		memcpy(buffer_recvdata+4,buffer+offset,(file_size-offset));
    		printf("it's the last segmentation %ld,the length is %d\n",strlen(buffer_recvdata+4),file_size-offset);
    	}

    	printf("the sending offset is %d,the number of segmentation is %d\n",offset,(n+1));
    	PDU_encode(p,buffer_recvdata, buffer_send,(data_length+4));
    	send_PDU(buffer_send,sizeof(buffer_send));
    	offset = offset + data_length;
    	//printf("the file_size-offset is %d\n",(file_size-offset));

    }


	return 0;
}


