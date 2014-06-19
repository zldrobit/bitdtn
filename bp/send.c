#include <stdio.h>
#include "bp.h"
#include "uri.h"

int main()
{
	int bpsock;
	struct URI dst_bp_endpoint_id;
	struct URI src_bp_endpoint_id;

	bpsock = bp_socket();
	uri_assign(&src_bp_endpoint_id, "bitdtn", "send");
	uri_assign(&dst_bp_endpoint_id, "bitdtn", "recv");
	bp_bind(bpsock, &src_bp_endpoint_id);
	bp_sendto(bpsock, &dst_bp_endpoint_id, "hello!", 7, 1);
	bp_close(bpsock);


}

