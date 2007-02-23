#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include "bloom.h"
#include "hashes.h"

#define hashsize(n) ((BIGNUM)1<<(n))
#define hashmask(n) (hashsize(n) - 1)

int bloom_init(bloom *bloom,BIGNUM size,int hashes,hash_t hash,int flags)
{

	if (size < 1) {
		fprintf(stderr,"overflow1\n");
		return -1;
	} else {
		/* this may waste a little time, but we need to ensure
		 * that our array has a prime number of elements, even
		 * if we have been requested to do otherwise */
		bloom->elements = find_close_prime(size);
	}
	if (hashes < 1) {
		fprintf(stderr,"hashes was %d,size %ld\n",hashes,size);
		return -1;
	} else {
		bloom->ideal_hashes = hashes;
	}

	if (hash == NULL) {
		bloom->hash = (hash_t)HASH_FUNC;
	} else {
		bloom->hash = hash;
	}

	if ((flags & VERBOSE) == VERBOSE) {
		fprintf(stderr,"bloom_init(%ld,%d) => (%ld,%d)\n",(BIGCAST)size,hashes,(BIGCAST)bloom->elements,bloom->ideal_hashes);
		verbose = 1;
	} else {
		verbose = 0;
	}
	if ((size > TOPLIMIT)) {
		fprintf(stderr,"overflow2\n");
		return -2;
	}

	/* allocate our array of bytes.  where m is the size of our desired 
	 * bit vector, we allocate m/8 + 1 bytes. */
	if ((bloom->vector = (char *)malloc(sizeof(char) * ((int)(bloom->elements / 8) + 1))) == NULL) {
		perror("malloc");
		return -1;
	}
	/* generate a collection of random integers, to use later
	 * when salting our keys before hashing them */
	sketchy_randoms(&bloom->random_nums,hashes);

	return 0;
}

void bloom_destroy(bloom *bloom)
{
	free(bloom->vector);
	free(bloom->random_nums.num);
}

int bloom_check(bloom *bloom,char *str)
{
	return bloom_test(bloom,str,RO);
}

int bloom_add(bloom *bloom,char *str)
{
	return bloom_test(bloom,str,SET);
}

int bloom_test (bloom *bloom,char *str,int mode)
{
	int i,hit;
	BIGNUM ret;

	/* as many times as our ideal hash count dictates, salt our key
	 * and hash it into the bit vector */	
	hit = 1;
	for (i=0; i < bloom->ideal_hashes; i++) {
		ret = bloom_hash(bloom,str,i);
		if (!test(bloom->vector,ret)) {
			hit = 0;
			if (mode == SET) {
				set(bloom->vector,ret);
			} else {
				/* if we are merely testing, we are done. */
				return hit;
			}
		}
	}
	return hit;
}

BIGNUM bloom_hash(bloom *bloom,char *str, int i)
{
	char *newstr;
	char salt[10];
	BIGNUM ret = 0;
	BIGNUM hash;

	if (i > 0) {
		sprintf(salt,"%d",bloom->random_nums.num[i]);
		if ((newstr = (char *)malloc(sizeof(char) * (strlen(str) + strlen(salt)  + 1))) == NULL) {
			perror("malloc");
			errno = ERR_MALLOC;
			return -1;
		}
		sprintf(newstr,"%s%s",str,salt);
	} else {
		newstr = strdup(str);
	} 

	/* as long as our hash function returns a positive # */	
	ret = (BIGNUM)abs(bloom->hash(newstr)) % (BIGNUM)bloom->elements;
	
	free(newstr);

	return ret;
}

int set(char *big,BIGNUM index)
{
	deref dr;
	
	finder(index,&dr);
	big[dr.index] += dr.spot;
	
	return 0;
}

int test (char *big, BIGNUM index)
{
	deref dr;
	char bucket;
	
	finder(index,&dr);
	bucket = big[dr.index];
	if ((bucket & dr.spot) == dr.spot) {
		return 1;
	} else {
		return 0;
	}
}

int finder (BIGNUM index,deref *dr) 
{

	dr->index = (BIGNUM)(index / 8);
	dr->spot = pow(2,(index % 8));	
	return 0;
}



int sketchy_randoms(randoms *rands,int cnt)
{
	int i;
	srand(CONS);
	if ((rands->num = (int *)malloc(sizeof(int) * (cnt+1))) == NULL) {
		perror("malloc");
		errno = ERR_MALLOC;
		return -1;
	}
	for (i=0; i < cnt;i++) {
		rands->num[i] = 1 + (int)((float)SALTRAND * rand() / (RAND_MAX + 1.0));
	}
	rands->cnt = cnt;
	return 0;
}

BIGNUM report_capacity(bloom *bloom)
{
	/* this is the slow way to do this! */
	BIGNUM i,cnt;
	
	cnt = 0;
	for (i=0;i<bloom->elements;i++) {
		if (test(bloom->vector,i)) {
			cnt++;
		}
	}
	return cnt;
}
