#include "stats.h"

static web_stats *stats;

int init_stats(void){
	if((stats = mmap(NULL, sizeof(web_stats) , PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED){
		perror("mmap");
		exit(1);
	}
	if(sem_init(&stats->sem, 1, 1) == -1){
		perror("sem_init");
		exit(1);
	}
	stats->served_connections = 0;
	stats->served_requests = 0;
	stats->ok_200 = 0;
	stats->ko_400 = 0;
	stats->ko_403 = 0;
	stats->ko_404 = 0;
	return 1;
}

web_stats *get_stats(void){
	return stats;
}
