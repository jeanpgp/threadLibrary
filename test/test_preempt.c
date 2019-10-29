#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

#define INTERVAL 5000000
#define END 300

int huge(void* arg)
{	
	printf("Running huge loop\n");
	long int i = 0;
	while(1) {
		if ((i % INTERVAL) == 0) {
			printf("Hello huge %ld!\n", i / INTERVAL);
		}
		i++;
	}

	return 3;
}

int hello_thread(void* arg) {
	printf("Hello thread %d!\n", uthread_self());

	if(uthread_self() == END) exit(0);

	uthread_create(hello_thread, NULL);	
	return 6;
}

int main(void)
{
	uthread_t tid, tid2;

	tid = uthread_create(huge, NULL);
	tid2 = uthread_create(hello_thread, NULL);

	uthread_join(tid, NULL);
	uthread_join(tid2, NULL);
	
	printf("thread%d\n", uthread_self());
	
	return 0;
}
