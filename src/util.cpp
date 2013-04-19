
#include "util.h"

int
atoi( char *val, int base ) {
	int num=0;
	int i = 0;
	for( ; *(val+i)<='9' && *(val+i)>='0' ; i++ ) 
		num = num*base + *(val+i) - '0' ;
	return num;
}

char*
itoa(int val, int base){	
	static char buf[32] = {0};	
	int i = 30;	
	for(; val && i ; --i, val /= base)	
		buf[i] = "0123456789abcdef"[val % base];	
	return &buf[i+1];	
}
