#ifndef TIME_H
#define TIME_H

#include <sys/time.h>

timeval start, endt;

void init_time(){
	gettimeofday(&start,NULL);
}


double get_time(){
	gettimeofday(&endt,NULL);
	return (1000000*(endt.tv_sec-start.tv_sec)+(endt.tv_usec-start.tv_usec))/1000000.0;
}

#endif
