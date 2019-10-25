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


struct uthread{
	int tid;
	ucontext_t context;
	char* stack;
	
};

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
	/* TODO Phase 2 */
	char stack[STACK_SIZE];
	ucontext_t data;
	queue_t q;
	q = queue_create();
	getcontext(&data);
	data.uc_stack.ss_sp   = stack;
	data.uc_stack.ss_size = STACK_SIZE;
	queue_enqueue(q, &data);
	return queue_length(q);
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
	/* TODO Phase 2 */
	/* TODO Phase 3 */
}

