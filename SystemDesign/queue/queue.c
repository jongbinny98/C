#include "queue.h"                     // header 

#include <pthread.h>                   // pthread
#include <stdio.h>                     
#include <stdlib.h>                    // malloc, free
#include <unistd.h>

/*
Queueblock to build the linkedlist.
*/
typedef struct node
{
    void *elem;
    struct node *next;
}node;

/*
queue has FIFO properties and should 
support multi producer and multi consumer
*/ 
typedef struct queue
{
    struct node *head;
    struct node *tail;
    int size;
    int current_size;
    pthread_mutex_t mutex;
    pthread_cond_t wakeup;
}queue_t;

/*
Allocates a queue with size `size`
*/
queue_t *queue_new(int size)
{
    queue_t *q = (queue_t*)malloc(sizeof(queue_t)); // allocate memory for queue

    if (q == NULL)               // check if queue is created
    {
        return NULL;
    }
    q->size = size;              // given max queue size
    q->current_size = 0;         // curent queue size 
    q->head = NULL;              // head of the queue points to NULL
    q->tail = NULL;              // tail of the queue points to NULL

    // initialize a pointer to mutex and wakeup is equal to 0 --> return queue
    if ((pthread_mutex_init(&q->mutex, NULL) == 0) && (pthread_cond_init(&q->wakeup, NULL) == 0))
    {
        return q;
    }
    // free(q);                     // free queue

    return 0;
}

/*
frees a queue (should assume the queue is empty)
*/
void queue_delete(queue_t **q)
{   
    // if the pthread_mutex_destroy is not successful
    if(pthread_mutex_destroy(&(*q)->mutex) != 0 || pthread_cond_destroy(&(*q)->wakeup) != 0)
        return;

    node *temp_node;
    temp_node = (*q)->head;

    if(temp_node != NULL)                 // if the temp_node is not empty, do optionally more deep free
    {
        while(temp_node->next != NULL)    // if next node is exist
        {
            (*q)->head = temp_node->next; // next == point to the node->next
            free(temp_node);              // free the current node
            temp_node = (*q)->head;       // node is pointing to the next node
        }
        free(temp_node);                  // when one  left.
    }
    free(*q);                       // if one node left --> free queue  
    // *q = NULL;
}

/*
1. add an element to the queue
2. blocks if queue is full as producer
*/
bool queue_push(queue_t *q, void *elem)
{
    if(q->current_size + 1 > q->size)
    {
        return 0;
    }
    node *newnode = (node*) malloc(sizeof(node));   // allocate memory for newnode
    if (newnode == NULL)                            // if newnode is not created
    {
        return 0;
    }
    newnode->elem = elem;
    newnode->next = NULL;

    pthread_mutex_lock(&q->mutex);                  // synchronization process one thread out of time
    
    if(q->head == NULL && q->tail == NULL)          // if head and tail pointer is pointing to NULL
    {                                               // points to newnode. --> first push
        q->head = q->tail = newnode;    
    }
    else                                            // if head and tail pointer is not pointing to NULL
    {                                               // newnode is already exist
        q->tail->next = newnode;                    // tail points to newnode.
        q->tail = newnode;
    }
    q->current_size += 1;

    pthread_mutex_unlock(&q->mutex);                // unlock mutex, now other thread can operate.
    pthread_cond_signal(&q->wakeup);                // signalled thread to wake up and immediately take the mutex

    return 0;
}

/*
remove an element from the queue
blocks if the queue is empty as consumer
*/
bool queue_pop(queue_t *q, void **elem)
{
    pthread_mutex_lock(&q->mutex);                  // synchronization process one thread out of time

    while(q->head == NULL)                          // blocks if the queue is empty as consumer
    {
        pthread_cond_wait(&q->wakeup, &q->mutex);
    }
    node* temp_node = q->head;                      // create temporary node and point to head of the queue
    *elem = temp_node->elem;                        // store the value of element of this temporary node
    q->head = q->head->next;                        // head points to next node

    q->current_size -= 1;
    if(q->current_size == 0)
    {
        q->tail = NULL;
    }
    if(q->current_size == q->size-1)
    {
        pthread_cond_broadcast(&q->wakeup);
    }
    // free(temp_node->elem);
    free(temp_node);                                // free the head pointer
    
    pthread_mutex_unlock(&q->mutex);                // unlock mutex, now other thread can operate.
    return 1;                                       // return true
}
