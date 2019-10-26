/*
 * Simple hello world test
 *
 * Tests the creation of a single thread and its successful return.
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int hello(void* arg)
{
	printf("Hello world!\n");
	uthread_self();

	return 0;
}

int main(void)
{
	uthread_t tid,tid2;

	tid = uthread_create(hello, NULL);
	tid2 = uthread_create(hello, NULL);
	uthread_self();
	uthread_join(tid, NULL);
	uthread_join(tid2, NULL);
	
	return 0;
}
