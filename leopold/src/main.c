#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include "bloom.h"

void usage()
{
	printf("sorry, world\n");
	exit(1);
}

int main(int argc,char *argv[])
{
	bloom bloom;
	char buf[4096];
	int opt,k;
	struct bloomstat stat;
	BIGNUM m,n;
	BIGNUM cnt;
	float e;
	
	m = stat.n = stat.e = k = n = e = 0;
	
	while ((opt = getopt(argc,argv,"m:k:e:n:")) != -1) {
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
		}
	}

	
	if (k && m) {
		/* we are ok */
	} else if (n && e) {
		get_suggestion(&stat,n,e);
		m = stat.m;
		k = stat.k;
	} else {
		usage();
	}

	if (bloom_init(&bloom,m,k,NULL,VERBOSE) != 0) {
		fprintf(stderr,"initialization failure\n");
		exit(2);
	}

	while(fgets(buf,sizeof(buf),stdin) != NULL) {
		trim_newline(&buf);
		if (bloom_add(&bloom,buf)) {
			printf("%s\n",buf);
		} else {
			//printf("%s",buf); 
		}
	}
	//cnt = report_capacity(&bloom);
	//fprintf(stderr,"cnt is %ld / %ld \n",cnt,(BIGCAST)bloom.elements);
	
	bloom_destroy(&bloom);
	return 0;
}


int trim_newline(char *string) {
   unsigned int i = 0;

       while(*(string + i) != '\0') {

	       if(*(string + i) == '\n') {
	               *(string + i) = '\0';
	                return 0;
	       }

              i++;
  }

return -1;
}

