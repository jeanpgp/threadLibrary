#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>
#include <ucontext.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"
#include "context.h"

#define STACK_SIZE 32768


struct uthread{
	int tid;
	ucontext_t context;
	char* stack;
	/* 0 running, 1 ready, 2 blocked, 3 zombie */
	int state;
};
int tid_idx = 0;

queue_t queue ;

/* TODO Phase 2 */

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	/* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg)
{
	/*If queue has not been malloced then malloc!!*/
	if (queue == NULL){
		queue = queue_create();
		//initializing main
		struct uthread* thread = (struct uthread*)malloc(sizeof(struct uthread));
		if (getcontext(&thread->context)){
			return -1;
		}
		thread->stack = uthread_ctx_alloc_stack();
	}

	/* TODO Phase 2 */
	int retval;
	char stack[STACK_SIZE];
	uthread_ctx_t uctx;
	retval = uthread_ctx_init(&uctx, stack, func, NULL);
	if (retval !=0){
		return -1;
	}
	
	struct uthread* thread = (struct uthread*)malloc(sizeof(struct uthread));	
	thread->tid = queue_length(queue)+1;
	thread->context = uctx;
	thread->stack = stack;
	queue_enqueue(queue, thread);
	
	

	return thread->tid;
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
}

int find_tid(void *data, void *arg)
{
	printf("tid: %d\n",*((int*)arg));
	struct uthread* thread = (struct uthread*)data;
	if (thread->tid == *(int*)arg){
		return 1;
	}
	return 0;
}

/*Context Switch*/
int uthread_join(uthread_t tid, int *retval)
{
	/* TODO Phase 2 */
	void *data;
	uthread_ctx_t main_context;
	/*(queue_t queue, queue_func_t func, void *arg, void **data)*/
	queue_iterate(queue, find_tid, &tid, &data);
	/* Get data which is the struct of next */
	/* uthread_ctx_switch(uthread_ctx_t *prev, uthread_ctx_t *next)*/	
	/* saves first parameter*/
	struct uthread* thread = (struct uthread*)data;
	uthread_ctx_switch( &main_context, &thread->context);
	/* TODO Phase 3 */
}

