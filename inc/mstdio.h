#ifndef _MSTDIO_H_
#define _MSTDIO_H_
#include <sys/types.h>


#define MEOF -1

enum mode{READ, WRITE, APPEND};

typedef struct
{
	int     _fd;
	char*   _buffer;
	char*   _nextc;
	int  	_mode;
	off_t   _left;
}MFILE; 

extern MFILE* 	mfopen(const char* const pathname,
						const char* const mode);
extern int 		mfclose(MFILE* fp);
extern MFILE* 	mfflush(MFILE *fp);
extern MFILE* 	mfdopen(int fd, const char* const mode);


extern int		mfgetc(MFILE* fp);
extern int 		mfputc(int character, MFILE* fp);
extern int		mungetc(int character, MFILE* fp);
extern char*	mfgets(char* buffer, int size, MFILE* fp);
extern int 		mfputs(char* buffer, MFILE* fp);

extern size_t	mfread(void* buffer, size_t size,
						size_t counter, MFILE* fp);
extern size_t 	mfwrite(void* buffer, size_t size,
						size_t count, MFILE *fp);						

#endif
