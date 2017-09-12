#include "job.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>
#include <malloc.h>
#include <sys/wait.h>
#include <sys/types.h>

char* prompt = "mshell>";
#define MAX_COMMAD_LEN 256

extern char** environ;

#define FOREGROUND 0
#define BACKGROUND 1


void parse_cmd(Job* job, char* line, int* bg)
{	
	char** args=(char**)calloc(100,  sizeof(char*));
	assert(args != NULL);
	char* cmd=strtok(line, " ");
	args[0] = (char*)calloc(strlen(cmd)+1,  sizeof(char));
	strcpy(args[0], cmd);
	int i = 1;
	char* s;

	Redirection* rs[5];
	int redirect_num= 0 ;

	*bg = FOREGROUND;

	while((s = strtok(NULL, " ")) != NULL){
	
		if(!strcmp(s, "&")){
			*bg = BACKGROUND;
			continue;
		}
	
	
		if(!strcmp(s, "<")){
			char* file = strtok(NULL, " ");
			if(file == NULL) continue;
			else{
				int fd = open(file, O_RDONLY);
				rs[redirect_num++] = 
						create_redirect(fd, RedirectRead);
			}
			continue;
		}
		if(!strcmp(s, ">")){
			char* file = strtok(NULL, " ");
			if(file == NULL) continue;
			else{
				int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
				rs[redirect_num++] = 
						create_redirect(fd, RedirectWrite);
			}
			continue;
		
		}
		if(!strcmp(s, ">>")){
			char* file = strtok(NULL, " ");
			if(file == NULL) continue;
			else{
				int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
				rs[redirect_num++] = 
						create_redirect(fd, RedirectAppend);
			}
			continue;
		
		}


		args[i] = (char*)calloc(strlen(s)+1, sizeof(char));
		strcpy(args[i], s);
		i++;
	}
	Program* prog = create_program(args);
	
	int k = 0;
	for(; k < redirect_num; k++){
		add_redirection(prog, rs[k]);
		destroy_redirect(rs[k]);
	}


	add_program(job, prog);
	int j;
	for(j = 0; j<i; j++){
		free(args[j]);
	}
	free(args);
}

void cd_fun(Program* prog)
{	
	if((chdir(prog->args[1]) < 0)){
		perror("cd error");
	}
}

void pwd_fun(Program* prog)
{
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	if(getcwd(buffer, sizeof(buffer)) == NULL){
		perror("pwd error");
	}
	printf("%s\n", buffer);
}

void exit_fun(Program* prog)
{
	exit(0);
}

void env_fun(Program* prog)
{
	int i = 0;
	char* env = NULL;
	while((env = environ[i]) != NULL){
		printf("%s\n", env);
		i++;
	}
}

void export_fun(Program* prog)
{
	if(prog->args[1] == NULL){
		fprintf(stderr, "export: invalid argument\n");
		return;
	}
	putenv(prog->args[1]);
}

void echo_fun(Program* prog)
{
	char* s = prog->args[1];
	if(s == NULL){
		fprintf(stderr, "echo: invalid argument\n");
		return;
	}
	if(s[0] == '$'){
		char* v = getenv(s+1);
		printf("%s\n", v);
	}else{
		printf("%s\n", s);
	}

}


void execute_cmd(Job* job, int bg)
{
	int i;
	for(i=0; i<job->progs_num; i++){
		if(!strcmp(job->progs[i].args[0], "cd")){
			cd_fun(&job->progs[i]);	
		}
		if(!strcmp(job->progs[i].args[0], "pwd")){
			pwd_fun(&job->progs[i]);
			return;
		}
		if(!strcmp(job->progs[i].args[0], "exit")){
			exit_fun(&job->progs[i]);
			return;
		}
		if(!strcmp(job->progs[i].args[0], "env")){
			env_fun(&job->progs[i]);
			return;
		}
		if(!strcmp(job->progs[i].args[0], "export")){
			export_fun(&job->progs[i]);
			return;
		}
		if(!strcmp(job->progs[i].args[0], "echo")){
			echo_fun(&job->progs[i]);
			return;
		}
		pid_t pid;
		if((pid = fork())<0){
			perror("fork error");
		}else if(pid == 0){
			
			signal(SIGINT, SIG_DFL);
			signal(SIGTSTP, SIG_DFL);
			signal(SIGTTOU, SIG_DFL);
			signal(SIGTTIN, SIG_DFL);
			signal(SIGCHLD, SIG_DFL);


			if(i == 0){
				if(setpgid(getpid(), getpid()) < 0){
					perror("setpgid error");
				}
				job->pgid = getpgid(getpid());
			}else{
				if(setpgid(getpid(), job->pgid) < 0){
					perror("setpgid error");
				}
			}
			if(bg == FOREGROUND){
				tcsetpgrp(0, getpgid(getpid()));
			}
	

			job->progs[i].pid = getpid();
			int k = 0;
			for(; k < job->progs[i].redirect_num; k++){
				if(job->progs[i].redirects[k].redirect == RedirectRead){
					if(dup2(job->progs[i].redirects[k].fd, STDIN_FILENO) != STDIN_FILENO){
						perror("dup2");
					}
				}
				if((job->progs[i].redirects[k].redirect == RedirectWrite) || (job->progs[i].redirects[k].redirect == RedirectAppend)){
					if(dup2(job->progs[i].redirects[k].fd, STDOUT_FILENO) != STDOUT_FILENO){
						perror("dup2");
					}
				}

			}

			if(execvp(job->progs[i].args[0], job->progs[i].args)<0){
				perror("execvp error");
				exit(1);
			}	
		}else{

			if(i == 0){
				if(setpgid(pid, pid) < 0){
					perror("setpgid error");
				}
				job->pgid = getpgid(pid);
			}else{
				if(setpgid(pid, job->pgid) < 0){
					perror("setpgid error");
				}
			}

			if(bg == FOREGROUND)
			{
				tcsetpgrp(0, job->pgid);
				waitpid(-job->pgid, NULL, WUNTRACED);
			}

			if(bg == BACKGROUND){
				waitpid(-job->pgid, NULL, WNOHANG);
			}
			/*
			if(waitpid(pid, NULL, WUNTRACED) < 0){
				perror("waitpid error");
			
			}*/
		}

		
	}
}

void sig_handler(int signo)
{
	if(signo == SIGCHLD){
		waitpid(-1, NULL,WNOHANG);
		tcsetpgrp(0, getpgid(getpid()));
	}
}

int main(void)
{

	setpgid(getpid(), getpid());
		
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGCHLD, sig_handler);

	char buffer[MAX_COMMAD_LEN];
	ssize_t size = strlen(prompt) * sizeof(char);
	write(STDOUT_FILENO, prompt, size);
	ssize_t len;
	int bg;
	while(1)
	{
		len = read(STDIN_FILENO, buffer, MAX_COMMAD_LEN);
		buffer[ len - 1]= 0;
		if(strlen(buffer) > 0){
			Job *job = create_job(buffer);
			parse_cmd(job, buffer, &bg);
			execute_cmd(job, bg);
			destroy_job(job);
		}
		write(STDOUT_FILENO, prompt, size);
		memset(buffer, 0, MAX_COMMAD_LEN) ;
	}
}
