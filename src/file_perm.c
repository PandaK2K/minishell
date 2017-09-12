#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

int main(int argc, char* argv[])
{
	if(argc < 2){
		fprintf(stderr, "usage: %s files\n", argv[0]);
		exit(1);
	}

	struct stat buff;
	int i;
	for(i=1; i<argc; i++){
		memset(&buff, 0, sizeof(buff));
		if(lstat(argv[i], &buff) < 0 ){
		perror("lsate err");
		continue;
		}
	
		mode_t mode = buff.st_mode;
		printf("%-20s", argv[i]);
	
		if(S_IRUSR & mode){
			printf("r");
		}else{
			printf("-");
		}
		if(S_IWUSR & mode){
			printf("w");
		}else{
			printf("-");
		}
		if(S_IXUSR & mode){
			printf("x");
		}else{
			printf("-");
		}
	
		if(S_IRGRP & mode){
			printf("r");
		}else{
			printf("-");
		}
		if(S_IWGRP & mode){
			printf("w");
		}else{
			printf("-");
		}
		if(S_IXGRP & mode){
			printf("x");
		}else{
			printf("-");
		}
	
		if(S_IROTH & mode){
			printf("r");
		}else{
			printf("-");
		}
		if(S_IWOTH & mode){
			printf("w");
		}else{
			printf("-");
		}
		if(S_IXOTH & mode){
			printf("x");
		}else{
			printf("-");
		}
		printf("\n");
	}
	

	return 0;
}
