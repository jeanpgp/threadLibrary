#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <uthread.h>
#include <queue.h>

void test_create(void)
{
	queue_t q;

	q = queue_create();
	assert(q != NULL);
	assert(queue_length(q) == 0);
}



void test_queue_simple(void)
{
    queue_t q;
    int data = 3, *ptr;
    q = queue_create();
    queue_enqueue(q, &data);
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data);
}

/*print and find 7 , Stop when found*/
int print(void *data, void *arg)
{
	printf("%d\n",*((int*)data));
	if (*(int*)data == 7){
		return 1;
	}
	return 0;

}

void test_queue_iterate(void)
{
	queue_t q;
	int data = 3, data2 = 7, data3 = 9, *ptr;
	
	queue_func_t print_ptr = &print;
	
	q = queue_create();
	queue_enqueue(q, &data);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	queue_iterate(q, *print_ptr, NULL,(void**)&ptr);
	assert(ptr == &data2);
	assert(queue_length(q) == 3);
	
}

void test_queue_delete(void)
{
	queue_t q;
	int data = 3,data2=6,data3=9, *ptr;

	q = queue_create();
	queue_enqueue(q, &data);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	queue_delete(q, &data);
	queue_dequeue(q, (void**)&ptr);
	assert(ptr == &data2);
	assert(queue_length(q) == 1);
	
}


void test_queue_error(void)
{
	queue_t q;
	int data = 3, data2 = 7, *ptr, r;
	
	q = NULL;
	/*q */
	assert(queue_length(q) == -1);
	/*dequeue with null q */
	r = queue_dequeue(q, (void**)&ptr);
	assert(r == -1);
	q = queue_create();
	/* dequeue without anything */
    	r = queue_dequeue(q, (void**)&ptr);
	assert(r == -1);
	queue_enqueue(q, &data);
	queue_enqueue(q, &data2);
	queue_dequeue(q, (void**)&ptr);
    	assert(ptr == &data);
	queue_dequeue(q, (void**)&ptr);
    	assert(ptr == &data2);
}

int main(void)
{
	test_create();
	test_queue_simple();
	test_queue_error();
	test_queue_delete();
	test_queue_iterate();
	return 0;
}
