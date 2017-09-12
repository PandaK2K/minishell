#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void term_fun1(void)
{
	printf("first term");
}
void term_fun2(void)
{
	printf("sec term");
}
void term_fun3(void)
{
	printf("third term");
}

int main(int argc, char* argv[])
{
	if(argc < 3){
		fprintf(stderr, "usage: %s file [exit|_exit|return]\n", argv[0]);
		exit(1);
	}

	atexit(term_fun1);
	atexit(term_fun2);
	atexit(term_fun3);

	FILE* fp = fopen(argv[1], "w");
	fprintf(fp, "hello world");
	if(!strcmp(argv[2], "exit")){
		exit(0);
	}else if(!strcmp(argv[2], "_exit")){
		_exit(0);
	}else if(!strcmp(argv[2], "return")){
		return 0;
	}else{
		fprintf(stderr, "usage: %s file [exit|_exit|return]\n", argv[0]);
		exit(1);
	}
	exit(0);
}
