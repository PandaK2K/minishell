#include <setjmp.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TOK_ADD 5
#define TOK_SUB 6

static jmp_buf env;

void do_line(char* line);
void cmd_add(void);
void cmd_sub(void);
int get_token(char* item);

char* prompt = "cal: ";

int main(void)
{
	ssize_t size = strlen(prompt) * sizeof(char);
	char buff[256];
	ssize_t len;
	
	if(setjmp(env) < 0){
		perror("setjmp error");
	}

	write(STDOUT_FILENO, prompt, size);
	while(1){
		len = read(STDIN_FILENO, buff, 256);
		if(len < 0){
			break;
		}
		buff[len - 1] = 0;
		do_line(buff);
		write(STDOUT_FILENO, prompt, size);

	}
}

void do_line(char* line)
{
	int cmd = get_token(line);
	switch(cmd){
		case TOK_ADD:
			cmd_add();
			break;
		case TOK_SUB:
			cmd_add();
			break;
		default:
			fprintf(stderr, "error command\n");
			
	}
}

void cmd_add(void)
{
	int i = get_token(NULL);
	int j = get_token(NULL);
	printf("result is %d\n", i+j);
}

void cmd_sub(void)
{
	int i = get_token(NULL);
	int j = get_token(NULL);
	printf("result is %d\n", i-j);

}

static int is_number(char* item)
{
	int len = strlen(item);
	int i;
	for(i = 0; i < len ; i++){
		if(item[i] > '9' || item[i] < '0' )
		return 0;
	}
	return 1;
}

int get_token(char* line)
{
	char* item = strtok(line, " ");
	if(line != NULL){
		if(!strcmp("add", item)) return TOK_ADD;
		if(!strcmp("sub", item)) return TOK_SUB;
	}else{
		if(is_number(item)){
			int i = atoi(item);
			return i;
		}else{
			fprintf(stderr, "arg not number\n");
			longjmp(env, 1);

		}
	}

}


