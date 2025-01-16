#include <stdio.h>
#include <stdlib.h>

#include "pool.h"

static void * routine(void *arg) {
    thread_pool_t *pool = (thread_pool_t *)arg;
    while(true) {
        printf("thread %#lx is waiting for task\n", pthread_self());
        task_t *task = NULL;
        if(task_queue_dequeue(pool->queue, &task) == -1) {
            continue;
        }
        printf("thread %#lx is executing task on connected socket %d\n", pthread_self(), task->fd);
        if( execute_task(task) == -1) {
            free_task(task);
            fprintf(stderr, "execute task failed\n");
            continue;  // 继续执行下一个任务
        }
        free_task(task);
        printf("execute task success\n");
    }
    return NULL;
}

thread_pool_t *thread_pool_create(int number, task_queue_t *queue) {
    thread_pool_t *pool = (thread_pool_t *)malloc(sizeof(thread_pool_t));
    if(pool == NULL) {
        return NULL;
    }
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * number);
    if(pool->threads == NULL) {
        free(pool);
        return NULL;
    }
    pool->thread_number = number;
    pool->queue = queue;

    // 创建线程
    for(int i = 0; i < number; i++) {
        if(pthread_create(&pool->threads[i], NULL, routine, pool) != 0) {
            // 创建线程失败：取消已创建的线程 
            for(int j = 0; j < i; j++) {
                pthread_cancel(pool->threads[j]);
            }
            free(pool->threads);
            free(pool);
            return NULL;
        }
    }
    return pool;
}
