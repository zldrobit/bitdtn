#include "cfdp.h"


void* rate_thread(void *ptr)
{
	
	 /*
	 int times = 0;
	 int rate;

	
		
		//while(1)
	 {
			
			   
               rate = number_of_received_pacekt/(times);
			   printf(" *****************the rate is %d************\n",rate);
			   times = times +1;
			   //sleep(3);
			}
		
		*/
	int times = 1;
	int rate;
	while(1)
	{
	sleep(3);
	rate = number_of_received_pacekt/times;
	printf("******************%d\n",rate);
	times++;
	}
}