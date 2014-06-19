#include <stdio.h>
#include "bundle.h"

int main()
{
	unsigned int value = 0xABC0;
	unsigned int s;
	unsigned int value2;

	sdnv_encode(&s, &value, 3);
	sdnv_decode(&value2, &s);
	printf("s = 0x%4X\n", s);
	printf("value2 = 0x%4X\n", value2);

	return 0;
}
