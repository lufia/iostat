#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "iostat.h"

int
main(void)
{
	DriveStats stats[NDRIVE];
	int i, n;

	n = readdrivestat(stats, NDRIVE);
	if(n < 0){
		fprintf(stderr, "can't read drive statistics: %s\n", strerror(errno));
		exit(1);
	}
	for(i = 0; i < n; i++){
		printf("name: %s\n", stats[i].name);
	}
}
