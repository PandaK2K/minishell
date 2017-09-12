#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	pid_t pid;
	pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(1);
	}else if(pid == 0){
		printf("pid: %d, ppid: %d\n", getpid(), getppid());
		exit(0);
	}
	while(1){
		sleep(1);
	}

	exit(0);

}
