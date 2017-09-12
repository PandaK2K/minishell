#include "io.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int fd_in,fd_out;
	int flag = 0;

	int i;
	for(i = 1; i<argc; i++){
		if(!strcmp("+", argv[i])){
			fd_in=open(argv[++i],O_RDONLY);
			if(fd_in<0){
				perror("open error");
				exit(1);
			};
			if(dup2(fd_in, STDIN_FILENO) != STDIN_FILENO){
				perror("dup2 error");
				exit(1);
			}

			close(fd_in);

		}else if(!strcmp("-",argv[i])){
			fd_out = open(argv[++i],O_WRONLY |O_CREAT | O_TRUNC, 0777 );
			if(fd_out < 0 ){
				perror("open error");
				exit(1);
			}
			if(dup2(fd_out, STDOUT_FILENO) != STDOUT_FILENO){
				perror("dup2 error");
				exit(1);
			}
			close(fd_out);
		}else{
			flag =1 ;
			fd_in= open(argv[i], O_RDONLY);
			if(fd_in < 0 ){
				perror("open error");
				exit(1);
			}
			if(dup2(fd_in, STDIN_FILENO) != STDIN_FILENO){
				perror("dup2 error");
				exit(1);
			}
			copy(STDIN_FILENO,STDOUT_FILENO);
			close(fd_in);	
		}
	}
	if(!flag)
		copy(STDIN_FILENO,STDOUT_FILENO);

	exit(1);

}
