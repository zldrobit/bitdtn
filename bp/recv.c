#include <stdio.h>
#include "bp.h"
#include "uri.h"

int main()
{
	int bpsock;
	struct URI dst_bp_endpoint_id;
	struct URI src_bp_endpoint_id;
	char buffer[100];

	bpsock = bp_socket();
	uri_assign(&src_bp_endpoint_id, "bitdtn", "recv");
	bp_bind(bpsock, &src_bp_endpoint_id);
	bp_recv(bpsock, buffer, 100);
	printf("%s\n", buffer);
	bp_close(bpsock);

}

