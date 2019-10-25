#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

/* individual node */
struct queue {
	/* TODO Phase 1 */
	struct node *head;
	struct node *tail;	
	int length;
};

struct node {
	struct node *next;
	struct node *prev;
	int *data;
};


queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t queue = (queue_t)malloc(sizeof(queue_t));
	queue->length = 0;
	return queue;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */	
	free(queue);
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	struct node* new_node = (struct node*)malloc(sizeof(struct node));
	
	if (queue == NULL || data == NULL || new_node == NULL){
		return -1;
	}
	new_node->data = data;
	new_node->next = NULL;
	/* If empty */
	if (queue->head == NULL){
		new_node->prev = NULL;
		queue->head = new_node;
	}
	else{/*if not empty */
		/*points to the head */
		struct node* curr_node = queue->tail;
		new_node->prev = curr_node;
		curr_node->next = new_node;	
	}
	queue->tail = new_node;
	queue->length = queue->length + 1;
	return 0;
	
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if (queue == NULL || queue->head == NULL || queue->head->data == NULL){
		return -1;
	}
	struct node* head = queue->head;
	*data =  head ->data;
	if (head->next == NULL){/* empty the queue */
		queue->head = NULL;
		queue->tail = NULL;
		queue->length = 0;
	}
	else{
		struct node* new_head = head->next;
		new_head->prev = NULL;
		queue->head = new_head;
		queue->length = queue->length - 1;
	}
	return 0;
	
}

int queue_delete(queue_t queue, void *data)
{
	struct node* curr_node = queue->head;
	while(curr_node->data !=NULL && curr_node!=NULL){
		if (curr_node->data == data){
			struct node* next = curr_node->next;
			if (curr_node->prev != NULL){
				struct node* previous = curr_node->prev;
				previous->next = next->prev;
			}
			else{
				next->prev = NULL;
				queue->head =  next;
			}
			queue->length = queue->length - 1;
			return 1;
		}
	}
	return -1;
	/* TODO Phase 1 */ 
}

/*
 * queue_func_t - Queue callback function type
 * @data: Data item
 * @arg: Extra argument
 *
 * Return: 0 to continue iterating, 1 to stop iterating at this particular item.
 */



int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO Phase 1 */
	struct node* curr_node = queue->head;
	int stop = 0;
	if (queue == NULL || func == NULL)
	{
		return -1;
	}
	while(curr_node != NULL )
	{/* while not the end */

		stop = func(curr_node->data, arg);
		/* End Prematurely */
		if (stop == 1 )
		{
			if (curr_node->data != NULL){
				*data = curr_node->data;
				return 0;
			}
		}
		curr_node = curr_node->next;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	if (queue == NULL)
	{
		return -1;
	}
	return queue->length;
	
}

