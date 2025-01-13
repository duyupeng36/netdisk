#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "pool.h"


static void *routine(void *arg) {
    fprintf(stdout, "Thread[TID %#lx] is starting, who is waiting for task\n", (long int)pthread_self());
    task_queue_t *queue = (task_queue_t *)arg;
    while(true) {
        task_t task;
        if(task_queue_pop(queue, &task) == -1) {
            // get task error
            fprintf(stderr, "get task error\n");
            continue;
        }
        fprintf(stdout, "Thread[TID %#lx] is handling task for fd: %d\n", (long int)pthread_self(), task.user->fd);
        if(task.handler(task.user, task.argc, task.argv) == -1) {
            // handle error
            fprintf(stderr, "handle error for fd: %d\n", task.user->fd);
            continue;
        }
        fprintf(stdout, "Thread[TID %#lx] has handled task for fd: %d\n", (long int)pthread_self(), task.user->fd);
    }
    return NULL;
}

int thread_pool_init(thread_pool_t *pool, int num_threads, task_queue_t *queue) {
    if(pool == NULL) {
        return -1;
    }

    pool->num_threads = num_threads;
    pool->threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    if(pool->threads == NULL) {
        return -1;
    }
    // 创建线程
    for(int i =0; i < num_threads; i++) {
        if(pthread_create(&pool->threads[i], NULL, routine, queue) != 0) {
            return -1;
        }
    }
    return 0;
}

int thread_pool_destroy(thread_pool_t *pool) {
    if(pool == NULL) {
        return -1;
    }
    free(pool->threads);
    return 0;
}

