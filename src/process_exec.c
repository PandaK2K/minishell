#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
char* cmd1 = "cat";
char* cmd2 = "/bin/cat";

char* argv1 = "/etc/passwd";
char* argv2 = "/etc/group";

int main(void)
{
	pid_t pid;
	if((pid = fork()) < 0){
		perror("fork error");
		exit(1);
	}else if(pid == 0){
		if(execl(cmd2, cmd1, argv1, argv2, NULL) < 0){
			perror("execl error");
			exit(1);
		}else{
			printf("execl %s success\n", cmd1);
		}
	}
	wait(NULL);
	
	printf("-----------------------------------------------");
}
