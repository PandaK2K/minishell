#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

void out_status(int status)
{
	if(WIFEXITED(status)){
		printf("normal exti: %d\n", WEXITSTATUS(status));
	}else if(WIFSIGNALED(status)){
		printf("abnormal exti: %d\n", WTERMSIG(status));
	}else if(WIFSTOPPED(status)){
		printf("STOP sig %d\n", WSTOPSIG(status));
	}else{
		printf("unkonw");
	}
}

int main(void)
{
	int status;
	pid_t pid;
	if((pid = fork()) < 0){
		perror("fork error");
		exit(1);
	}else if(pid == 0){
		printf("pid: %d, ppid: %d\n", getpid(), getppid());
//		sleep(100);
		exit(0);
	}

	wait(&status);
	out_status(status);
	printf("---------------------------------------\n");
	exit(0);
}
