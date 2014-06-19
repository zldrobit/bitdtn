#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "bp.h"
#include "uri.h"

#define USOCK_PATH_PREFIX 	"/tmp/bp_"
#define STR_LEN			20	

void itoa(unsigned int i, char* str)
{
	int j,k;
	char t;

	if (i == 0){
		str[0] = '0';
		str[1] = '\0';
		return ;
	}

	j = 0;
	while (i > 0){
		str[j] = i % 10 + '0';
		i /= 10;
		j++;
	}
	str[j] = '\0';
	for (k = 0; k < j/2; k++){
		t = str[k];
		str[k] = str[j - k - 1];
		str[j - k - 1] = t;
	}
}

static char* pid_usock_path(char* suffix)
{
	static char str[STR_LEN];
	char str2[STR_LEN];
	strcpy(str, USOCK_PATH_PREFIX);
	// printf("%s\n", str);
	itoa(getpid(), str2);
	// printf("str2 = %s\n", str2);
	strcat(str, str2);
	// printf("%s\n", str);
	if (suffix != NULL){
		// printf("suffix = %s\n", suffix);
		strcat(str, "_");
		strcat(str, suffix);
	}
	// printf("%s\n", str);
	return str;
}

int bp_socket()
{
	static int index = 0;
	int usock;
	struct sockaddr_un uaddr;
	char str[STR_LEN];

	usock = socket(AF_UNIX, SOCK_DGRAM, 0);
	uaddr.sun_family = AF_UNIX;
	itoa(index, str);
	// printf("str = %s (0)\n", str);
	strcpy(uaddr.sun_path, pid_usock_path(str));
	// printf("%s\n", uaddr.sun_path);
	if (bind(usock, (struct sockaddr*) &uaddr, sizeof(uaddr)) == -1){
		printf("bp_socket() usock bind failed!\n");
	}
	// unlink(uaddr.sun_path);
	index++;
	return usock;
}

void bp_close(int bp_sockfd)
{
	struct BPD_CLOSE signal_close;
	struct sockaddr_un bpd_uaddr;

	signal_close.signal_type = BPD_SIGNAL_CLOSE;
	bpd_uaddr.sun_family = AF_UNIX;
	strcpy(bpd_uaddr.sun_path, BPD_USOCK_ADDR);
	sendto(bp_sockfd, &signal_close, sizeof(signal_close), 0,
		(struct sockaddr*) &bpd_uaddr, sizeof(bpd_uaddr));
	close(bp_sockfd);	
}

int bp_bind(int bp_sockfd, struct URI* bp_endpoint_id_ptr)
{
	struct BPD_BIND signal_bind;
	struct sockaddr_un bpd_uaddr;

	signal_bind.signal_type = BPD_SIGNAL_BIND;
	// strcpy(signal_bind.bp_endpoint_id, bp_endpoint_id);
	uri_copy(&signal_bind.bp_endpoint_id, bp_endpoint_id_ptr);
	bpd_uaddr.sun_family = AF_UNIX;
	strcpy(bpd_uaddr.sun_path, BPD_USOCK_ADDR);
	sendto(bp_sockfd, &signal_bind, sizeof(signal_bind), 0,
		(struct sockaddr*) &bpd_uaddr, sizeof(bpd_uaddr));
}

void bp_sendto(int bp_sockfd, struct URI* dst_bp_endpoint_id_ptr, 
	char* buffer, unsigned int len, int iscustody)
{
	struct BPD_SEND signal_send;
	struct sockaddr_un bpd_uaddr;
	struct URI src_bp_endpoint_id;
	struct URI report_to_bp_endpoint_id;
	struct URI custodian_bp_endpoint_id;

	signal_send.signal_type = BPD_SIGNAL_SEND;
	bpd_uaddr.sun_family = AF_UNIX;
	strcpy(bpd_uaddr.sun_path, BPD_USOCK_ADDR);
	// strcpy(signal_send.dst_bp_endpoint_id, 
	// 	dst_bp_endpoint_id);
	uri_copy(&signal_send.dst_bp_endpoint_id,
		dst_bp_endpoint_id_ptr);
	// signal_send.lifetime = lifetime;
	uri_assign(&src_bp_endpoint_id, "", "");
	uri_assign(&report_to_bp_endpoint_id, "", "");
	uri_assign(&custodian_bp_endpoint_id, "", "");
	uri_copy(&signal_send.src_bp_endpoint_id, 
		&src_bp_endpoint_id);
	uri_copy(&signal_send.report_to_bp_endpoint_id, 
		&report_to_bp_endpoint_id);
	uri_copy(&signal_send.custodian_bp_endpoint_id, 
		&custodian_bp_endpoint_id);
	memcpy(signal_send.payload, buffer, len);
	signal_send.payload_block_length = len;
	signal_send.iscustody = iscustody;
	sendto(bp_sockfd, &signal_send, sizeof(signal_send), 0,
		(struct sockaddr*) &bpd_uaddr, sizeof(bpd_uaddr));
}

ssize_t bp_recv(int bp_sockfd, char* buffer, int len)
{
	return recv(bp_sockfd, buffer, len, 0);
}

