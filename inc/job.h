#ifndef _JOB_H_
#define _JOB_H_

#include <sys/types.h>

enum RedirectType{RedirectRead, RedirectWrite, RedirectAppend};

typedef struct
{
	enum RedirectType redirect;
	int fd;
}Redirection;

typedef struct
{
	pid_t pid;
	char** args;
	Redirection* redirects;
	int redirect_num;
}Program;



typedef struct
{
	char *cmd;
	int progs_num;
	Program *progs;
	pid_t pgid;
}Job;

Job* create_job(char* cmd);
void destroy_job(Job* job);
Program* create_program(char** arg);
void destroy_program(Program *prog);
int add_program(Job *job, Program* prog);

Redirection* create_redirect(int fd, enum RedirectType type);
void destroy_redirect(Redirection* r);
void add_redirection(Program* prog, Redirection* r);

#endif
