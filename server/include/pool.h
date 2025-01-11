#ifndef POOL_H
#define POOL_H

#include <pthread.h>
#include "task.h"

typedef struct thread_pool {
    int num_threads;
    pthread_t *threads;
} thread_pool_t;

/**
 * @brief Initialize a thread pool
 * 
 * @param pool pointer to the thread pool, must be allocated, which will be initialized
 * @param num_threads number of threads to create
 * @param queue pointer to the task queue, which will be used by the threads
 * @return on success, return 0, otherwise return -1
 */
int thread_pool_init(thread_pool_t *pool, int num_threads, task_queue_t *queue);

/**
 * @brief Destroy a thread pool
 * 
 * @param pool pointer to the thread pool to destroy, which will be destroyed
 * @return on success, return 0, otherwise return -1
 */
int thread_pool_destroy(thread_pool_t *pool);
#endif

