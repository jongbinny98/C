Jongbin Baek
asgn2

------------------------------------------------------------------------------------------------------
queue.c:
In this assignment, we build the bounded buffer to get used to synchronization between multiple threads.
The bounded buffer will have 4 major function that is basically creating a queue with linkedlist. One important properities of queue is that "First in First out".
1. typedef struct queue queue_t --> will create the structure of a queue containing head and tail that is going to pointing the front and the rear. Also, it will define the size and the current_size to tract of the size in the queue as we increment and decrement the queue. Lastly, we have mutex and wakeup to be able to use for multithread environment.
2. queue_t *queue_new(int size) --> will create the queue that will be called by the main function in the test_files. As one of the maior function, it will create the default queue with the dynamic allocation memory "malloc"
3. queue_delete(queue_t **q) --> will free the queue and it will assume that the queue is empty. In this function, if the node is not empty, it will do the optionally more deep free process.
4. bool queue_push(queue_t *q, void *elem) --> will create a newnode and allocate required memory to push the node in the queue. And it will increment as we push to the queue. All this process will be operated by one thread out of time by using pthread_mutex_lock and unlock. and after unlock, it will signal other thread to use it.
5. bool queue_pop(queue_t, void **elem) --> as similar to the push, all the process will be operated by one thread out of time, which means that other thread won't be able to interrupt when one thread is working in this fucntion. In this function, it will create temp_node to pop the element of the head. if the head is NULL, which if the queue is empty, then the consumer is going to wait.
6. #include <pthread.h> --> safety pthread
- In this program, one of the recommended library was to use pthread.h so that we get to use synchronization between multiple threads. 
- pthread_create(,,) --> create thread and run the function that is taken from the thrid arguement.
- pthread_join(,) --> a thread will wait until the function is done working and will go back to the main with the return value(second argument)
- pthread_mutex_init(,) --> initlize a pointer to mutex variable.
- pthread_mutec_destory() --> done with mutex
- pthread_mutex_lock() --> lock for synchronization so that other thread won't affect when processing
- pthread_mutex_unlock() --> unlock so that other thread can do the process
- pthread_mutex_signal() --> signal others to let know to join it now.
- pthread_cond_wait() --> atomic unlock and wait 
--------------------------------------------------------------------------------------------------------
Makefile
1. In this time, the make file will create queue.o object file to use for testing with the main.c file that is given to us.
In the asgn2 directory, you are supposed to have queue.c and queue.h file and you will use command "make" to have queue.o file. After that, run ./filename.sh to check if it is succeed or not. if it prints 0, then it means succeed. if non zero, then it means failed.
------------------------------------------------------------------------------------------------------
test_files: 
- There were errors in the test_files in casting int. Therefore, I have changed casting int to long and it worked. For example, at line 18 in basic-push-pop.c file, if ((int)r != 1) { changed to if ((long)r != 1) {. 
I have included all the given test and change from int to long just in case.

2. test_files: 
- a. push-string-pop.c: to check if other type of element is working on this bounded buffer. 
- b. thread-order.c: to check to push when the queue is full.
- 
3. test_script:
- a. push-string-pop.sh: testscript to use push-string-pop to check if it works. if echo $? return 0, then it means succeed. if nonzero, then it fails. Also, it check if there is a memory leaks after we push and pop the elements.
- b. thread-order.sh: testscript to use for thread-order.c to check if it works, it will print 0 if successful.
- 

------------------------------------------------------------------------------------------------------