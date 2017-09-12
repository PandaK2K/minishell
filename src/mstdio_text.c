#include <stdio.h>
#include "mstdio.h"
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <memory.h>

int main(void)
{
	MFILE *fp1 = mfopen("/etc/passwd", "r");
	assert(fp1 != NULL);

	MFILE *fp2 = mfopen("mypasswd", "w");
	assert(fp2 != NULL);

//	char* buff;
//	buff=(char*)calloc(100,sizeof(char));
//	read(fp1->_fd, buff, 100);
//	write(STDOUT_FILENO, buff, 100);

	char c;
	while((c = mfgetc(fp1)) != MEOF){
//		printf("%c", c);
//	write(STDOUT_FILENO, &c, 1);
		mfputc(c, fp2);
	}
	mfclose(fp1);
	mfclose(fp2);
	return 0;

}
