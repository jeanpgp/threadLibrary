/*
 * Thread yielding, exiting, and joining test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int thread3(void* arg)
{
	printf("Arrive thread3 Hi\n");
	uthread_yield();
	printf("thread%d\n", uthread_self());
	int retval = 0;
	uthread_exit(retval);
	return 3;
}

int thread2(void* arg)
{
	printf("Arrive thread2 Hi\n");
	uthread_join(uthread_create(thread3, NULL), NULL);
	uthread_yield();
	printf("thread%d\n", uthread_self());
	return 2;
}

int thread1(void* arg)
{
	uthread_create(thread2, NULL);
	printf("Arrive thread1 Hi\n");
	return 1;
	
	int retval = 0;
	uthread_exit(retval);
	printf("thread%d\n", uthread_self());
	uthread_yield();
	return 1;
}

int main(void)
{
	int* retval = malloc(sizeof(int));
	
	uthread_join(uthread_create(thread1, NULL), retval);
	printf("retval thread1: %d\n", *retval);
	printf("thread%d\n", uthread_self());
	
	free(retval);
	return 0;
}
