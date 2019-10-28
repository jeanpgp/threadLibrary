#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 * 0.01 seconds, or 10,000 milliseconds is T, period
 */
#define HZ 100
#define T 10000

void timer_handler(int signum) {
	printf("handled");
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
}

void preempt_start(void)
{
	struct sigaction sa;
	struct itimerval timer;
	
	/* Allocate memory */
	memset(&timer, 0, sizeof(timer));
	memset (&sa, 0, sizeof (sa));

	/* Set timer_handler as signal handler for timer */
 	sa.sa_handler = &timer_handler;
 	sigaction(SIGVTALRM, &sa, NULL);
 
	/* Set timer to raise alarm every time period elapses */
	timer.it_value.tv_sec = 0;
	timer.it_interval.tv_sec = 0;
	timer.it_value.tv_usec = T;
	timer.it_interval.tv_usec = T;
	
	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

