#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"
#include <bloom.h>
#include "const-c.inc"
typedef bloom Bloomer;

MODULE = Bloom::Faster	PACKAGE = Bloom::Faster

INCLUDE: const-xs.inc

Bloomer *
binit(size,hashes)
	unsigned long size
	int hashes
PREINIT: 
	bloom *newbloom;
CODE:
	if ((newbloom = (bloom *)malloc(sizeof(bloom))) == NULL) {
		perror("malloc");
	}
	if (bloom_init(newbloom,size,hashes,NULL,0) != 0) {
		RETVAL = NULL;
	} else {
		RETVAL = newbloom;
	}
OUTPUT:
	RETVAL


Bloomer *
binit_sugg(n,e)
	char *n;
	char *e;
PREINIT: 
	bloom *newbloom;
CODE:
	struct bloomstat stat;
	unsigned long real_n = atoll(n);
	double real_e = atof(e);
	
	get_suggestion(&stat,real_n,real_e);
	if ((newbloom = (bloom *)malloc(sizeof(bloom))) == NULL) {
		perror("malloc");
	}
	if (bloom_init(newbloom,stat.m,stat.k,NULL,1) != 0) {
		RETVAL = NULL;
	} else {
		RETVAL = newbloom;
	}
OUTPUT:
	RETVAL


void
suggestion(n,e,m,k)
	char *n;
	char *e;
	SV * m;
	int k;
PROTOTYPE: $$
CODE:
	char holder[1000];
	struct bloomstat stat;
	unsigned long real_n = atoll(n);
	double real_e = atof(e);
	
	get_suggestion(&stat,real_n,real_e);

	sprintf(holder,"%lld",stat.m);
	m = newSVpvn(holder,strlen(holder));
	k = stat.k;
OUTPUT:
	m	
	k

void
bloom_destroyer(newbloom)
	Bloomer *newbloom
PREINIT:
	bloom *thisbloom;
CODE:
	thisbloom = (bloom *)newbloom;
	bloom_destroy(thisbloom);
	free(thisbloom);

static int
test_bloom(newbloom,str,mode)
	Bloomer *newbloom
	char *str;
	int mode;
PREINIT:
	bloom *thisbloom;
CODE:
	thisbloom  = (bloom *)newbloom;
	RETVAL = bloom_test(thisbloom,str,mode);
OUTPUT:
	RETVAL


