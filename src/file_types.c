#include <sys/types.h>
#include <sys/stat.h>
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
	
	int i =1;
	struct stat buff;

	for(i=1; i<argc; i++){
		memset(&buff, 0, sizeof(buff));
		if(lstat(argv[i], &buff) < 0 ){
			perror("lsate err");
			continue;
		}
		printf("%-20s", argv[i]);
		if(S_ISREG(buff.st_mode)){
			printf("normal file");
		}else if(S_ISDIR(buff.st_mode)){
			printf("directory");
		}else if(S_ISBLK(buff.st_mode)){
			printf("block device");
		}else if(S_ISSOCK(buff.st_mode)){
			printf("sock device");
		}else if(S_ISFIFO(buff.st_mode)){
			printf("named pipe");
		}else if(S_ISLNK(buff.st_mode)){
			printf("link file");
		}else{
			printf("unkown type");
		}
		printf("\n");




	}
	

	return 0;
}
