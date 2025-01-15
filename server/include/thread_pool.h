#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdbool.h>

#include <pthread.h>

#include "queue.h"

typedef struct thread_pool
{
    pthread_t *threads;     // 线程数组
    int thread_number;      // 线程数量
    task_queue_t *queue;   // 任务队列
}thread_pool_t;

/**
 * @brief 创建线程池
 * 
 * @param number 线程数量
 * @param queue  任务队列
 * @return thread_pool_t* 成功返回线程池指针，失败返回 NULL 
 */
thread_pool_t *thread_pool_create(int number, task_queue_t *queue);

#endif // THREAD_POOL_H