#include <job.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

Job* create_job(char* cmd)
{
	Job* job= (Job*)malloc(sizeof(Job));
	assert(job != NULL);
	job->cmd = (char*)malloc(sizeof(char) * strlen(cmd));
	assert(job->cmd!=NULL);
	strcpy(job->cmd, cmd);
	job->progs_num = 0;
	job->progs= NULL;
	return job;

}

void destroy_job(Job* job)
{
	assert(job != NULL);
	free(job->progs);
	free(job->cmd);
	free(job);

}

static int arg_num(char** arg)
{
	int i=0;
	char* start = arg[0];
	while(start != NULL){
		i++;
		start= arg[i];

	}
	return i;
}

Program* create_program(char** arg)
{
	Program* prog = (Program*)malloc(sizeof(Program));
	assert(prog != NULL);
	prog->redirects = NULL;
	prog->redirect_num = 0;
	int counter = arg_num(arg);
	prog->args = (char**)calloc(counter+1, sizeof(char*));
	int i;
	for(i=0; i<counter;i++){
		int len = strlen(arg[i]);
		prog->args[i] = (char*)malloc(len);
		assert(prog->args[i] != NULL);
		strcpy(prog->args[i], arg[i]);
	}
	prog->args[i] = NULL;
	return prog;

}
void destroy_program(Program *prog)
{
	assert(prog != NULL);
	int i= 0;
	while((prog->args[i] != NULL)){
		free(prog->args[i]);
		i++;
	}
	free(prog->redirects);
	free(prog->args);
	free(prog);
}
int add_program(Job *job, Program* progs)
{
	Program* ps = (Program*)malloc(sizeof(Program) * (job->progs_num+1));
	memcpy(ps, job->progs, job->progs_num * sizeof(Program) );
	ps[job->progs_num++] = *progs;
	free(job->progs);
	job->progs = ps;

}

Redirection* create_redirect(int fd, enum RedirectType type)
{
	Redirection* r = (Redirection*)calloc(1, sizeof(Redirection));
	assert(r != NULL);
	r->fd = fd;
	r->redirect = type;
	return r;
}
void destroy_redirect(Redirection* r)
{
	assert(r != NULL);
	free(r);
}
void add_redirection(Program* prog, Redirection* r)
{	
	Redirection* rs = (Redirection*)calloc(prog->redirect_num + 1, 
									sizeof(Redirection));
	assert(rs != NULL);
	if(prog->redirects != NULL){
		memcpy(rs, prog->redirects,
				prog->redirect_num * sizeof(Redirection));
				free(prog->redirects);
	}
	prog->redirects = rs ;
	memcpy(&prog->redirects[prog->redirect_num], r, sizeof(Redirection));
	prog->redirect_num +=1;
}


