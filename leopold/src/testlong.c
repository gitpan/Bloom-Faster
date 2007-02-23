#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "bloom.h"


int main(int argc,char *argv[])
{
	BIGNUM foo;

	foo = (BIGNUM)atoll(argv[1]);

	printf("sizeof long is %d\n",sizeof(long));
	printf("sizeof long long is %d\n",sizeof(long long));
	printf("got %ld\n",(BIGNUM)foo);
	printf("ll max is %lld\n",LLONG_MAX);

	if (foo < foo-1) {
		printf("overflow");
		return 1;
	}
	return 0;
}
