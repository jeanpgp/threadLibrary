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

#define STACK_SIZE 32768
typedef struct _uthread_t uthread_t;

struct uthread{
	uthread_t tid;
	uthread_ctx_t* context;
	char* stack;
	/* 0 ready, 1 running, 2 blocked, 3 zombie */
	int state;
};

queue_t queue;
queue_t zombie;
uthread_t* running_t;
uthread_t* main_t;
uthread_t tid_idx = 0;
uthread_t my_tid = 0;

int find_tid(void *data, void *arg)
{
	struct uthread* thread = (struct uthread*)data;
	if (thread->tid == *(uthread_t*)arg){
		return 1;
	}
	return 0;
}

void uthread_yield(void)
{
	/* Init next to store next thread to be running, and curr to running thread */
	void *next;
	void *curr = running_t;
	
	/* Pop next in line and set to running thread */
	queue_dequeue(queue, &next);
	running_t = (struct uthread*)next;

	/* Switch context between curr and next */
	my_tid = (*next)->tid;
	uthread_ctx_switch(curr->context, next->context);
	
}

uthread_t uthread_self(void)
{
	/* TODO Phase 2 */
	return my_tid;

}

int create_main()
{
	queue = queue_create();
	zombie = queue_create();

	//initializing main
	uthread_ctx_t* uctx = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
	struct uthread* thread = (struct uthread*)malloc(sizeof(struct uthread));
	if (getcontext(uctx)){
		return -1;
	}

	thread->context = uctx;
	thread->tid = tid_idx;
	thread->stack = uthread_ctx_alloc_stack();
	thread->state = 1; /* Running */
	tid_idx++;
	main_t = thread;
	return 0;
}

int uthread_create(uthread_func_t func, void *arg)
{
	/*If queue has not been malloced then malloc!!*/
	if (tid_idx == 0){
		create_main();
	}
	/* TODO Phase 2 */
	int retval;
	void* stack = uthread_ctx_alloc_stack();
	uthread_ctx_t* uctx = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
	retval = uthread_ctx_init(uctx, stack, func, NULL);
	if (retval !=0){
		return -1;
	}	
	struct uthread* thread = (struct uthread*)malloc(sizeof(struct uthread));

	thread->tid = tid_idx;
	thread->context = uctx;
	thread->stack = stack;
	thread->state = 0; /* Ready */
	tid_idx++;
	queue_enqueue(queue, thread);
	return thread->tid;
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
	uthread_yield();
	
}


/*Context Switch*/
int uthread_join(uthread_t tid, int *retval)
{
	/* TODO Phase 2 */
	void *data;
	void *main_;
	uthread_t main_tid = 0;
	uthread_ctx_t main_context;
	/*(queue_t queue, queue_func_t func, void *arg, void **data)*/
	/* Get data which is the struct of next */
	/* uthread_ctx_switch(uthread_ctx_t *prev, uthread_ctx_t *next)*/	
	/* saves first parameter*/
	/* Yield from running go back to queue */
	while (1)
	{
		printf("main:%d\n",(*main_t)->tid);
		/* get of the ready list -> running */
		queue_dequeue(queue, &data);
		running_t = data;

		struct uthread* thread = (struct uthread*)data;
		thread->state = 1;
		/* main yield to thread1 */
		my_tid = thread->tid;
		uthread_ctx_switch((*main_t)->context, thread->context);
	}
	/* TODO Phase 3 */
	return 0;
}

