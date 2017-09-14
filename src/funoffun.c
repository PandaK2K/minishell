#include <stdio.h>
#include <stdlib.h>
typedef struct
{
int	num;
}Stu;

typedef int (*fun)(int num);

Stu* afun(int num)
{
	Stu* stu;
	stu = (Stu*)calloc(1,sizeof(Stu));
	stu->num = num;
	return stu;
}

int fun1(int num){
	num = 2;
	return num;
}

int fun2(int num){
	num = 1;
	return num;
}

void useFun(fun x)
{
	int num = 0;
	
	printf("num is %d", x(num));
}

int main(void)
{
	int num = 1;
	printf("%d\n", afun(num)->num);
	
	useFun(fun2);

	return 0;
}
