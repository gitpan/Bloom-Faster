#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include "bloom.h"


int main(int argc,char *argv[])
{
	int opt,k;
	struct bloomstat stat;
	BIGNUM m,n;
	float e;
	char mode = 's';
	
	m = stat.n = stat.e = k = n = e = 0;
	
	while ((opt = getopt(argc,argv,"pm:k:e:n:")) != -1) {
		switch(opt) {
			case 'm':
				m = (BIGNUM)atoll(optarg);
				break;
			case 'k':
				k = atoi(optarg);
				break;
			case 'n':
				n = (BIGNUM)atoll(optarg);
				break;
			case 'e':
				e = atof(optarg);
				break;
			case 'p':
				mode = 'p';
				break;
		}
	}

	
	if (k && m) {
		/* we are ok */
	} else if (n && e) {
		if (mode == 's') {
			get_suggestion(&stat,n,e);
			printf("%lld\t%lld\t%d\n",(long long)n,(long long)stat.m,stat.k);
		} else {
			m = find_close_prime(n);
			printf("%lld\t%lld\n",(long long)n,(long long)m);
		}
		m = stat.m;
		k = stat.k;
	} else {
		//usage();
	}

}

