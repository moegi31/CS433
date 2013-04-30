#include <cstdio>
#include <iostream>
#include <cstring>
#include <openssl/des.h>

DES_LONG ctol(unsigned char *c)
{
        /* The long integer */
    DES_LONG l;

    l =((DES_LONG)(*((c)++)));
        l = l | (((DES_LONG)(*((c)++)))<<8L);
        l = l | (((DES_LONG)(*((c)++)))<<16L);
        l = l | (((DES_LONG)(*((c)++)))<<24L);
        return l;
};


int main()
{

	printf("size of char: %lu\n", sizeof(char));
	
	printf("size of DES_LONG: %lu\n", sizeof(DES_LONG));

	unsigned char poop[5] = "abcd";
	DES_LONG block = ctol(poop);

	printf("block is: %u\n", block);
	return 0;
}
