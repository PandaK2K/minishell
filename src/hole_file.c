#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

char *buffer = "012346789";

int main(int argc, char* argv[])
{

	if(argc<2){
	fprintf(stderr, "usage: %s [file] \n", argv[0]);
	exit(1);
	}
	
	int fd = open(argv[1], O_WRONLY | O_CREAT |O_TRUNC, 0777);
	if(fd <0 ){
	perror("open error");
	exit(1);
	}

	size_t size = strlen(buffer) * sizeof(char);
	if(write(fd, buffer, size) !=size  ){
	perror("write error");
	exit(1);
	}

	if(lseek(fd, 10L, SEEK_END) < 0){
	perror("lseek error");
	exit(1);
	}

	if(write(fd, buffer, size) !=size  ){
	perror("write error");
	exit(1);
	}
	
	close(fd);
	exit(1);

}
