#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char** environ;

void show_env(void)
{
	int i = 0;
	char* env;
	while( (env = environ[i]) != NULL ){
		printf("%s\n", env);
		i++;
	}
}

int main(void)
{
	show_env();
	return 0;

}
