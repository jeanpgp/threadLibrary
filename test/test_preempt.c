#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

long int MAX = 10000;

int disable(void* arg)
{	
	printf("Running infinite loop\n");
	while(1) {}

	printf("Hello world!\n");
	uthread_self();

	return 0;
}

int main(void)
{
	uthread_t tid;

	tid = uthread_create(disable, NULL);
	uthread_self();
	uthread_join(tid, NULL);
	
	return 0;
}
