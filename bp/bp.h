#ifndef _BP_H_
#define _BP_H_

#include <pthread.h>
#include "uri.h"
#include "bundle.h"

#define BPD_USOCK_ADDR  	"/tmp/bpd"
// which is defined by IANA
#define BUNDLE_PORT     	4556
#define BP_EID_MAXLEN		2050
#define DTN_NONE		"dtn:none"
#define SECS_EPOCH_TO_2000	946684800

extern pthread_mutex_t bpd_creation_sequence_number_mutex;
extern unsigned int bpd_creation_sequence_number_counter;

enum BPD_STATUS{
	BPD_SUCCESS,
	BPD_FAIL,
};

#define BPD_BIND_BODY \
	struct URI bp_endpoint_id;

#define BPD_CLOSE_BODY \
	;

#define BPD_SEND_BODY \
	BUNDLE_BODY

#define BPD_RECV_BODY \
	;

// #define BPD_BIND_REQ_BODY \
// 	struct URI bp_endpoint_id;
// 
// #define BPD_BIND_RESP_BODY \
// 	enum BPD_STATUS status;
// 
// #define BPD_CLOSE_REQ_BODY \
// 	;
// 
// #define BPD_CLOSE_RESP_BODY \
// 	enum BPD_STATUS status;
// 
// #define BPD_SEND_REQ_BODY \
// 	BUNDLE_BODY
// 
// #define BPD_SEND_RESP_BODY \
// 	enum BPD_STATUS status;
// 
// #define BPD_RECV_RESP_BODY \
// 	BUNDLE_BODY
	
extern int bpd_usock, bpd_isock;

enum BPD_SIGNAL_TYPE{
	BPD_SIGNAL_BIND,
	BPD_SIGNAL_CLOSE,
	BPD_SIGNAL_SEND,
	BPD_SIGNAL_RECV,
};
	
struct BPD_BIND{
	enum BPD_SIGNAL_TYPE signal_type;
	BPD_BIND_BODY
};

struct BPD_CLOSE{
	enum BPD_SIGNAL_TYPE signal_type;
	BPD_CLOSE_BODY
};

struct BPD_SEND{
	enum BPD_SIGNAL_TYPE signal_type;
	BPD_SEND_BODY
};

struct BPD_RECV{
	enum BPD_SIGNAL_TYPE signal_type;
	BPD_RECV_BODY
};

// enum BPD_SIGNAL_TYPE{
// 	BPD_SIGNAL_BIND_REQ,
// 	BPD_SIGNAL_BIND_RESP,
// 	BPD_SIGNAL_CLOSE_REQ,
// 	BPD_SIGNAL_CLOSE_RESP,
// 	BPD_SIGNAL_SEND_REQ,
// 	BPD_SIGNAL_SEND_RESP,
// 	BPD_SIGNAL_RECV_RESP,
// };
// 
// struct BPD_BIND_REQ{
// 	enum BPD_SIGNAL_TYPE signal_type;
// 	BPD_BIND_REQ_BODY
// };
// 
// struct BPD_BIND_RESP{
// 	enum BPD_SIGNAL_TYPE signal_type;
// 	BPD_BIND_RESP_BODY
// };
// 
// struct BPD_CLOSE_REQ{
// 	enum BPD_SIGNAL_TYPE signal_type;
// 	BPD_CLOSE_REQ_BODY
// };
// 
// struct BPD_CLOSE_RESP{
// 	enum BPD_SIGNAL_TYPE signal_type;
// 	BPD_CLOSE_RESP_BODY
// };
// 
// struct BPD_SEND_REQ{
// 	enum BPD_SIGNAL_TYPE signal_type;
// 	BPD_SEND_REQ_BODY
// };
// 
// struct BPD_SEND_RESP{
// 	enum BPD_SIGNAL_TYPE signal_type;
// 	BPD_SEND_RESP_BODY
// };
// 
// struct BPD_RECV_RESP{
// 	enum BPD_SIGNAL_TYPE signal_type;
// 	BPD_RECV_RESP_BODY
// };


struct BPD_SIGNAL{
	enum BPD_SIGNAL_TYPE signal_type;
	union {
		struct {BPD_BIND_BODY};
		struct {BPD_CLOSE_BODY};
		struct {BPD_SEND_BODY};
		struct {BPD_RECV_BODY};
	};
};

// struct BPD_SIGNAL{
// 	enum BPD_SIGNAL_TYPE signal_type;
// 	union {
// 		struct {BPD_BIND_REQ_BODY};
// 		struct {BPD_BIND_RESP_BODY};
// 		struct {BPD_CLOSE_REQ_BODY};
// 		struct {BPD_CLOSE_RESP_BODY};
// 		struct {BPD_SEND_REQ_BODY};
// 		struct {BPD_SEND_RESP_BODY};
// 		struct {BPD_RECV_RESP_BODY};
// 	};
// };

int bp_sokcet();
void bp_close(int bp_sockfd);
int bp_bind(int bp_sockfd, struct URI* bp_endpoint_id_ptr);
void bp_sendto(int bp_sockfd, struct URI* dst_bp_endpoint_id_ptr,
        char* buffer, unsigned int len, int iscustody);
ssize_t bp_recv(int bp_sockfd, char* buffer, int len);

void bpd_process_admin_record(struct BUNDLE* bundle_ptr);

#endif

