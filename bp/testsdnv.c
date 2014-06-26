#include <stdio.h>
#include "bundle.h"

int main()
{
	int i;
	unsigned int value = 0xABC0;
	char value2[4] = "\x80\x80\x82\x7f";
	unsigned int s;
	unsigned char encoded[8] = {0};
	unsigned int toencode = 200;

	// unsigned int value2;
	unsigned int decoded = 0;

	// sdnv_encode(&s, &value, 3);
	// sdnv_decode(&value2, &s);
	// printf("s = 0x%4X\n", s);
	// printf("value2 = 0x%4X\n", value2);

	// sdnv_decode(&decoded, value2, &s);
	// printf("decoded = %d\n", decoded);

	sdnv_encode(&encoded, &toencode, 4);
	// printf("encoded = 0x%02X%02X%02X%02X\n", (unsigned)encoded[0], 
	// 	(unsigned)encoded[1], 
	// 	(unsigned)encoded[2], 
	// 	(unsigned)encoded[3]);
	printf("encoeded is:\n");
	for (i = 0; i < 8; i++){
		printf("%02X ", encoded[i]);
	}
	printf("\n");

	return 0;
}
