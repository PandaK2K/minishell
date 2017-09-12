#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	pid_t group1, group2;
	setpgid(getpid(), getpid());
	group1 = getpgid((getpid()));

	pid_t pid;
	int i = 0;
	for(; i < 3; i++){
		pid = fork();
		if(pid < 0){
			perror("fork error");
			exit(1);
		}else if(pid > 0){
			if(i == 0){
				setpgid(pid, pid);
				group2 = getpgid(pid);
			}
			if(i == 1){
				setpgid(pid, group2);
				tcsetpgrp(0 ,group2);
			}
			if(i == 2){
				setpgid(pid, group2);
			}
		}else{
			if(i == 0){
				setpgid(getpid(), getpid());
				group2 = getpgid(getpid());
			}
			if(i == 1){
				setpgid(getpid(), group2);
			}
			if(i == 2){
				setpgid(getpid(), group1);
			}
			break;
		}
	}

	printf("pid:%d, ppid:%d, pgid:%d\n", getpid(), getppid(), getpgid(0));
	pause();
	exit(0);
}
