#include <stdio.h>
#include <string.h>
#include "bp.h"
#include "uri.h"

void itoa(unsigned int i, char* str);

int main()
{
	int bpsock;
	struct URI dst_bp_endpoint_id;
	struct URI src_bp_endpoint_id;
	int i;
	char tmp[100];

	bpsock = bp_socket();
	uri_assign(&src_bp_endpoint_id, "bitdtn", "send");
	uri_assign(&dst_bp_endpoint_id, "bitdtn", "recv");
	bp_bind(bpsock, &src_bp_endpoint_id);
	for (i = 0; i < 100; i++){
		itoa(i+1, tmp);
		bp_sendto(bpsock, &dst_bp_endpoint_id, tmp, strlen(tmp)+1, 1);
	}
	bp_close(bpsock);


}

