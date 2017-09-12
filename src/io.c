#include "io.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_LEN 1024

void copy(int fdin, int fdout)
{

	char buffer[BUFFER_LEN];
	ssize_t size;
	while( ( size = read(fdin,buffer, BUFFER_LEN))  > 0){
		if(write(fdout, buffer, size) != size ){
			fprintf(stderr, "write error: %s\n", strerror(errno) );
			exit(1);
		}
	}

	if(size < 0){
	
			fprintf(stderr, "write error: %s\n", strerror(errno) );
			exit(1);
		}
}

void set_fl(int fd, int flag)
{
	int val = fcntl(fd, F_GETFL);
	val |= flag;
	if(fcntl(fd, F_SETFL, val) < 0){
		perror("fcntl error");
	}
}
 
void clr_fl(int fd, int flag)
{
	int val = fcntl(fd, F_GETFL);
	val &= ~flag;
	if(fcntl(fd, F_SETFL, val) < 0){
		perror("fcntl error");
	}
}

int lock_reg(int fd, int cmd, short type, 
				off_t offset, short whence, off_t length)
{
	struct flock flock;
	flock.l_type = type;
	flock.l_start = offset;
    flock.l_whence = whence;
	flock.l_len = length;
	if(fcntl(fd, cmd, &flock) < 0 ){
		perror("fcntl error");
		return 0;
	}
	return 1;
}				


