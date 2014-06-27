/*
 * CNAK.c
 *
 *  Created on: Mar 31, 2014
 *      Author: wenting
 */





#include <stdio.h>
#include <netinet/in.h>    // for sockaddr_in
#include <arpa/inet.h>
#include <sys/types.h>    // for socketjiushi
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for printf
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
#include "cfdp.h"
#include "../bp/bp.h"
#include "../bp/uri.h"



//#define SELFISOCKADDR_IP "0.0.0.0"


//int selfSocket;
//struct sockaddr_in self_addr;


//int current_TSN;
//int selfSocket;
//struct sockaddr_in self_addr;

//////////////here we suggest the transaction is one to one,then we just consider the TSN///////////////////
///////////////NAK is set as 109 bytes/////////////////////////////
int TSN;

int bpsock;

struct URI dst_bp_endpoint_id;
struct URI src_bp_endpoint_id;








int send_PDU(char *buffer,int PDU_size){


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////bind to send the file/////////////////////////////////////////////////////

	//int peerSocket;
	//struct sockaddr_in peer_addr;

	//bind_socket(peerSocket,peer_addr, selfSocket, self_addr);




	//bzero(&peer_addr, sizeof(peer_addr));
	//peer_addr.sin_family = AF_INET;
	//peer_addr.sin_port = htons(a);
	//peer_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


	/////////////bind to send the file////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	//if(sendto(selfSocket, buffer, PDU_size, 0, (struct sockaddr*)&peer_addr, sizeof(peer_addr))>0)
		printf("sending PDU:have send the PDU %d\n",PDU_size);


		usleep(1000);
		bp_sendto(bpsock, &dst_bp_endpoint_id, buffer, PDU_size,0);
		//printf("the dst_bp_endpoint_id is %s:%s\n",dst_bp_endpoint_id.scheme,dst_bp_endpoint_id.ssp);

	return 0;
}






int lost_segment_detection(){



	return 0;
}



int transaction_indication(){
	;
}

int metadata_recv_indication(){
	;
}

int file_segment_recv_indication(){
	;
}

void init(int argc,char**argv){

	TSN = 0;
	bzero(CFDP_buffer_NAK,1000);
	metadata_flag = 0;
/*
	if((selfSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero(&self_addr,sizeof(self_addr)); //把一段内存区的内容全部设置为0
	self_addr.sin_family = AF_INET;    //internet协议族
	self_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	self_addr.sin_port = htons(0);

	if(bind(selfSocket, (struct sockaddr*)&self_addr, sizeof(self_addr)) < 0)
	{
		perror("connect\n");
		printf("self bind error\n");
		exit(1);
	}
*/
	       bpsock = bp_socket();
			uri_assign(&src_bp_endpoint_id, "bitdtn", argv[1]);
			bp_bind(bpsock, &src_bp_endpoint_id);
}


int main(int argc, char **argv){

	pthread_t tid_recv_thread;
	void* ret_recv;
	init(argc,argv);
	pthread_create(&tid_recv_thread, NULL, recv_thread, NULL);

	char file_name[FILE_NAME_MAX_SIZE+1];
	char dest_add[1];
	//char buffer[1024];
	//bzero(buffer,1024);
	char recv_file_name[] = {'a','b','c'};
	bzero(file_name, FILE_NAME_MAX_SIZE+1);
	printf("please input the destination address:\t");
	scanf("%s",dest_add);
	 uri_assign(&dst_bp_endpoint_id, "bitdtn", dest_add);
	printf("Please Input File Name On Server:\t");
	scanf("%s", file_name);
	cfdp_put_request(1,file_name,recv_file_name);
	pthread_join(tid_recv_thread, &ret_recv);
	return 0;

}








