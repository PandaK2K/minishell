#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
void* th_fn(void* arg)
{

	int distance = (int)arg;
	int i;
	for(i = 1; i<= distance; i++){
		printf("%lx run %d\n", pthread_self(), i);
		int time = (int)(drand48() * 100000);
		usleep(time);
	}

	return (void*)0;
}

int main(void)
{
	int err;
	pthread_t rabbit, turtle;

	if((err = pthread_create(&rabbit, NULL, th_fn, (void*)50)) != 0 ){
		perror("pthread_create error");
	}
	
	if((err =  pthread_create(&turtle, NULL, th_fn, (void*)50)) != 0){
		perror("pthread_create error");
	}

	sleep(10);

	printf("control thread id: %lx\n", pthread_self());
	printf("finished!\n");

	return 0;
}
