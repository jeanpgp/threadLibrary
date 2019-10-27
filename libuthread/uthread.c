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

queue_t queue;
queue_t running;
queue_t main_queue;
queue_t zombies;

struct uthread{
	uthread_t tid;
	uthread_ctx_t* context;
	char* stack;
	/* 0 ready, 1 blocked, 2 running, 3 zombie */
	int state;
};

uthread_t tid_idx = 0;
uthread_t my_tid = 0;

void run_next_thread(void** curr) 
{
	void *data;
	
	queue_dequeue(queue, &data); //pop the next in line;
	queue_enqueue(running, data);

	queue_enqueue(queue, *curr); 
	
	struct uthread* curr_t = (struct uthread*)(*curr);
	struct uthread* thread = (struct uthread*)data;
	my_tid = thread->tid;
	uthread_ctx_switch( curr_t->context, thread->context);;
}

int find_tid(void *data, void *arg)
{
	struct uthread* thread = (struct uthread*)data;
	if (thread->tid == *(uthread_t*)arg){
		return 1;
	}
	return 0;
}

/* TODO Phase 2 */

void uthread_yield(void)
{
	/* TODO Phase 2 */
	void *data;
	void *curr;
	
	queue_dequeue(queue, &data); //pop the next in line;
	queue_enqueue(running, data);

	/* Dequeue running */	
	queue_dequeue(running, &curr); 
	queue_enqueue(queue, curr); 
	
	struct uthread* curr_t = (struct uthread*)curr;
	struct uthread* thread = (struct uthread*)data;
	my_tid = thread->tid;
	uthread_ctx_switch( curr_t->context, thread->context);
	
}

uthread_t uthread_self(void)
{
	/* TODO Phase 2 */
	return my_tid;

}

int create_main()
{
	queue = queue_create();
	running = queue_create();
	main_queue = queue_create();
	zombies = queue_create();
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
	queue_enqueue(main_queue, thread);
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

//FIXME: retrieve return value
void uthread_exit(int* retval)
{
	/* Pull thread out of running and store */
	void* curr;
	queue_dequeue(running, &curr);
	struct uthread* curr_t = (struct uthread*)curr;
	
	/* Store thread in zombies */
	queue_enqueue(zombies, curr);
	
	/* Run next thread */
	run_next_thread(&curr);
}

int check_thread_done(uthread_t tid)
{
	int done = 1;
	void* data;
	for(int i = 0; i < queue_length(queue); i++)
	{
		queue_dequeue(queue, &data);
		if (((struct uthread*)data)->tid == tid) done = 0;
		queue_enqueue(queue,data);
	}
	return done;
}

/*Context Switch*/
int uthread_join(uthread_t tid, int *retval)
{
	/* TODO Phase 3 */
	/* What code should do:
	 * Get all info about current running thread, that is the parent
	 * set parent state to blocked (1)
	 * set child to running
	 * loop where all threads in queue run if they have ready status
	 * break loop when child is not in queue or is in zombies
	 * if child is in zombies, retrieve return value
	 * then set status to ready (0)
	 */
	/* Return error if thread tries to join with main */
	if (tid == 0) return -1;
	 
	void* parent;
	void* child;
	void* prev = NULL;
	
	/* Set parent to parent thread, either running or main */
	if(queue_length(running) != 0) {
		queue_dequeue(running, &parent);
	} else {
		queue_dequeue(main_queue, &parent);
		queue_enqueue(main_queue, parent);
	}
	
	/* Set parent thread state to blocked */
	struct uthread* parent_t = (struct uthread*)parent;
	parent_t->state = 1;
	int is_child_done = 0;
	
	/* Run other threads until the child finishes and parent can begin */
	while(!is_child_done) {
		/* Get next ready thread*/
		while (1) {
			queue_dequeue(queue, &child);
			if (((struct uthread*)child)->state != 0) {
				queue_enqueue(queue, child);
			} else break;
		}

		/* Change child state to running */
		struct uthread* child_t = (struct uthread*)child;
		child_t->state = 2;
		
		/* remove current running and set child to running */
		if (queue_length(running) > 0) {queue_dequeue(running, &prev)};
		queue_enqueue(running, child);
		my_tid = child_t->tid;
		
		/* Switch context to correct context
		 * Old context will be parent if no previous running process
		 */
		if(prev == NULL) {
			uthread_ctx_switch(parent_t->context, child_t->context);
		} else {
			uthread_ctx_switch(((struct uthread*)prev)->context, child_t->context);
		}
		
		/* Check if child has finished executing */
		is_child_done = check_thread_done(tid);
	}
	// TODO: get retval and free memory of child
	queue_enqueue(queue, parent);
	
	return 0;
}
