#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bloom.h"


BIGNUM hash3 (char *s) 
{
	BIGNUM i;
       	for( i=0; *s; s++ ) i = 131*i + *s; return i ;
}

BIGNUM hash1(char *str) {
	BIGNUM hash = 5381;
	BIGNUM c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

BIGNUM hash2(char *str)
{
	BIGNUM hash = 0;
        BIGNUM c;

        while ((c = *str++))
	        hash = c + (hash << 6) + (hash << 16) - hash;

        return hash;
}



