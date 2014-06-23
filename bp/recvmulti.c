#include <stdio.h>
#include <string.h>
#include "bp.h"
#include "uri.h"

int isrecved[101] = {0};
int nrecved = 0;

int main()
{
	int bpsock;
	struct URI dst_bp_endpoint_id;
	struct URI src_bp_endpoint_id;
	char buffer[100];
	int i;

	bpsock = bp_socket();
	uri_assign(&src_bp_endpoint_id, "bitdtn", "recv");
	bp_bind(bpsock, &src_bp_endpoint_id);
	while (1){
		bp_recv(bpsock, buffer, 100);
		i = atoi(buffer);
		if (!isrecved[i]){
			isrecved[i] = 1;
			nrecved++;
		}
		printf("first recv %s\n", buffer);
		if (nrecved == 100){
			break;
		}
	}
	printf("ALL recved!\n");
	bp_close(bpsock);

}

