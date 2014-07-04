#include "cfdp.h"
#include "time.h"


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
	int temp = 0;
	while(1)
	{
	sleep(1);
	rate = number_of_received_pacekt - temp;
	temp = number_of_received_pacekt;
	printf("******************%d\n",rate);
	times++;


	////////////in the un_nak mode//////////////////////
	///////////////check if the file is recieved all///////////////////
	if(workmode == 0)
	{
		int segment_number;		
		struct rate_arguments *rateargu;
		rateargu = (struct rate_arguments *)malloc(sizeof(struct rate_arguments));
		rateargu = ptr;
	        if(rateargu->meta.segmentation_control == 0)
	        {
	
    
			   if(rateargu->meta.file_size%data_length == 0)
				   {
		   			segment_number = rateargu->meta.file_size/data_length;

	   				}
	 			  else
	  			 {
		 			  segment_number = rateargu->meta.file_size/data_length+1;
	 			  }
	        
			printf("the segment number is %d\n",segment_number);    
			if((number_of_received_pacekt == segment_number)&&EOF_flag == 1)
			{
				if(write_flag == 0)
				{				
					write_file(rateargu->meta,rateargu->fp,recieve);
					write_flag = 1;
				}
			}
		}

		if(segment_number == (number_of_received_pacekt-1))
		{
			int i;
			for(i = 0;i<segment_number;i++)
			{
				if(CFDP_buffer_NAK[i] != 1)
					printf("the missing part is %d\n",i);
			}

		}


	}
}
}
