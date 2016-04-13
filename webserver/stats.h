#ifndef __STATS_H__
# define __STATS_H__
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <semaphore.h>

typedef struct {
	int served_connections ;
	int served_requests ;
	int ok_200 ;
	int ko_400 ;
	int ko_403 ;
	int ko_404 ;
	sem_t sem;
} web_stats ;



int init_stats(void);
web_stats *get_stats(void);

#endif
