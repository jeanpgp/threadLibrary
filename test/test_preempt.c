#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

long int MAX = 10000;

int huge(void* arg)
{	
	printf("Running huge loop\n");
	for (int i = 0; i < MAX; i++) {
		printf("Hello huge!");
	}

	printf("thread%d", uthread_self());

	return 3;
}

int hello_world(void* arg) {
	for (int i = 0; i < 10; i++) {
		printf("Hello world!");
	}
	
	return 6;
}

int main(void)
{
	uthread_t tid, tid2;

	tid = uthread_create(huge, NULL);
	tid2 = uthread_create(hello_world, NULL);

	uthread_join(tid, NULL);
	uthread_join(tid2, NULL);
	
	printf("thread%d\n", uthread_self());
	
	return 0;
}
