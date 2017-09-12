#include "io.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	if(argc < 4){
		printf("usage: content file locktype %s\n", argv[0]);
		exit(1);
	}

	ssize_t size = strlen(argv[1]) * sizeof(char);
	int fd = open(argv[2], O_WRONLY | O_CREAT, 0777);
	if(fd < 0){
		perror("open erro");
		exit(1);
	}
	sleep(5);

	if(!strcmp("lock", argv[3]))
				WRITE_LOCKW(fd, 0, SEEK_SET, 0);
	printf("lock success\n");
	printf("pid = %d \n",getpid());

	char* p = argv[1];
	int i;
	for(i = 0; i< size; i++){
		if(write(fd, (p+i), 1 != 1)){
			perror("write error");
			exit(1);
		}
	
	printf("%d success write one character\n", getpid());
	sleep(1);
	}

	if(!strcmp("unlock", argv[3]))
		UNLOCK(fd, 0, SEEK_SET, 0);
	printf("unlock success\n");
	printf("unlock pid: %d\n", getpid());
	close(fd);

	exit(1);
}
