/*
 * time.h
 *
 *  Created on: Apr 30, 2014
 *      Author: wenting
 */

#ifndef TIME_H_
#define TIME_H_

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

extern pthread_mutex_t mtx ;
extern pthread_cond_t cond ;
extern pthread_mutex_t nak_mtx ;
extern pthread_cond_t nak_cond ;
extern pthread_mutex_t meta_mtx ;
extern pthread_cond_t meta_cond ;


struct rate_arguments{

	struct meta_data meta;
	struct recv_inf recieve;
	FILE* fp;

};

#endif /* TIME_H_ */
