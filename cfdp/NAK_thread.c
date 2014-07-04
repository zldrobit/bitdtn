/*
 * NAK_thread.c
 *
 *  Created on: Apr 1, 2014
 *      Author: wenting
 */


#include "cfdp.h"


extern pthread_t tid_rate_thread;
 void* NAK_thread(void *ptr){

	struct NAK_arguments *nakargu;
	nakargu = (struct NAK_arguments *)malloc(sizeof(struct NAK_arguments));
	nakargu = ptr;
	printf("in the NAK thread :the retrans number is %d,nakflag is %d\n",nakargu->nakflag->buffer[0],nakargu->nakflag->flag);

	  pthread_mutex_t nak_mtx = PTHREAD_MUTEX_INITIALIZER;
	  pthread_cond_t nak_cond = PTHREAD_COND_INITIALIZER;

	struct timespec ts;
	int ret;
	////////////////////////////sending NAK and turn on the NAK timer(here we set the timer 3s)//////////////////////
	while(nakargu->nakflag->flag == 1){


		struct NAK_flag *nakflag;
		nakflag = (struct NAK_flag *)malloc(sizeof(struct NAK_flag));
		nakflag->flag = 0;
		struct NAK nak;
							struct PDU_header *p_nak;
							unsigned char NAK_buffer[4*NAK_number+23];
							p_nak = (struct PDU_header *)malloc(sizeof(struct PDU_header));
							p_nak->version = Version;
							p_nak->type = file_dir;
							p_nak->dir = to_file_recv;
							p_nak->mode = acknowledged;
							p_nak->CRCflag = not_present;
							p_nak->reserved1 = 0;
							p_nak->reserved2 = 0;
							p_nak->reserved3 = 0;
							p_nak->PDUdatalength = 2;
							p_nak->length_of_entity_ID = 7;
							p_nak->length_of_TSN = 32;
							p_nak->source_ID = nakargu->p->destination_ID;
						    p_nak->TSN = 1;
							p_nak->destination_ID = nakargu->p->source_ID;


							nak.directive_code = 8;

							//memcpy(nak.NAK_offset,nakargu->nakflag->buffer,nakargu->nakflag->buffer[0]+1);
							int i;/////////////////////for test///////////////////////
							for(i =0;i<nakargu->nakflag->buffer[0];i++)
							{

								printf("the retranmit is %d\n",nakargu->nakflag->buffer[i+1]);
							}
							memcpy(nak.NAK_offset,nakargu->nakflag->buffer,4*NAK_number);
							printf("the retransmit number is %d\n",nak.NAK_offset[0]);
							int k;
							for(k=0;k<nak.NAK_offset[0];k++)
							{
								printf("the sending NAK is %d\n",nak.NAK_offset[k]);
							}
							NAK_PDU(nak,NAK_buffer,nakargu->p );
							send_PDU(NAK_buffer,sizeof(NAK_buffer));

		clock_gettime(CLOCK_REALTIME,&ts);   //获取时间
		ts.tv_sec += 5;   //经过3秒钟
		ret = pthread_cond_timedwait(&nak_cond,&nak_mtx,&ts);
		if (ret == ETIMEDOUT){  //超时

			printf(" NAK ETIMEDOUT\n");
			check_missing(nakargu->meta,nakflag);
			nakargu->nakflag =nakflag;
			
			printf("the nakflag is %d\n",nakflag->flag);
			if(nakflag->flag == 0)
			{
				write_file(nakargu->meta,nakargu->fp,nakargu->recieve)	;
				pthread_cancel(tid_rate_thread);
              			  printf("the missing part is recieved all\n");
             			   //pthread_cond_destroy(&nak_cond);
				return NULL;
		
				break;
			}
			else
			{
				
				NAK_PDU(nak,NAK_buffer,nakargu->p );
				send_PDU(NAK_buffer,sizeof(NAK_buffer));
			}


		}

		else {    //未超时  有中断

			printf("NAK INTERRUPT\n");
			//pthread_cond_destroy(&nak_cond);
			return NULL;
			break;


		}



	}
}


