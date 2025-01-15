#include <stdlib.h>

#include "queue.h"

task_queue_t *task_queue_crate(int n) {
    int cap = (int)1 << n;
    if(cap < 0) {
        return NULL;
    }
    task_queue_t *queue = (task_queue_t *)malloc(sizeof(task_queue_t));
    if (queue == NULL) {
        return NULL;
    }
    queue->tasks =  (task_t **)calloc(cap, sizeof(task_t *));
    if (queue->tasks == NULL) {
        free(queue);
        return NULL;
    }
    queue->rear = 0;    // 队尾
    queue->front = 0;   // 队头
    queue->cap = cap;   // 队列大小
    queue->operat = QUEUE_DEQUEUE;  // 上次操作为出队
    // 初始化互斥锁
    if(pthread_mutex_init(&queue->mutex, NULL) != 0) {
        free(queue->tasks);
        free(queue);
        return NULL;
    }
    // 初始化条件变量
    if(pthread_cond_init(&queue->cond, NULL) != 0) {
        pthread_mutex_destroy(&queue->mutex);
        free(queue->tasks);
        free(queue);
        return NULL;
    }
    return queue;
}

void task_queue_destroy(task_queue_t *queue) {
    if(queue == NULL) {
        return;
    }
    // 释放所有任务
    for(int i = queue->front; i != queue->rear; i = (i + 1) & (queue->cap - 1)) {
        free_task(queue->tasks[i]);
    }    

    // 释放任务数组
    free(queue->tasks);
    // 销毁互斥锁
    pthread_mutex_destroy(&queue->mutex);
    // 销毁条件变量
    pthread_cond_destroy(&queue->cond);
    // 释放队列
    free(queue);
}

bool task_queue_is_empty(task_queue_t *queue) {
    // 队列为空的条件：队尾等于队头，且上次操作为出队
    return queue->rear == queue->front && queue->operat == QUEUE_DEQUEUE; 
}

bool task_queue_is_full(task_queue_t *queue) {
    // 队列已满的条件：队尾等于队头，且上次操作为入队
    return queue->rear == queue->front && queue->operat == QUEUE_ENQUEUE;
}


int task_queue_enqueue(task_queue_t *queue, task_t *task) {
    if(queue == NULL || task == NULL) {
        return -1;
    }
    // 加锁
    if(pthread_mutex_lock(&queue->mutex) != 0) {
        return -1;
    }
    while (task_queue_is_full(queue)) {
        // 队列已满，等待条件变量
        if(pthread_cond_wait(&queue->cond, &queue->mutex) != 0) {
            pthread_mutex_unlock(&queue->mutex);
            return -1;
        }
    }
    // 入队
    queue->tasks[queue->rear] = task;
    queue->rear = (queue->rear + 1) & (queue->cap - 1);
    // 设置上次操作为入队
    queue->operat = QUEUE_ENQUEUE;
    // 解锁
    if(pthread_mutex_unlock(&queue->mutex) != 0) {
        return -1;
    }
    // 通知等待条件变量的线程
    if(pthread_cond_signal(&queue->cond) != 0) {
        return -1;
    } 
    return 0;
}

int task_queue_dequeue(task_queue_t *queue, task_t **task) {
    if(queue == NULL || task == NULL){
        return -1;
    }
    // 加锁
    if(pthread_mutex_lock(&queue->mutex) != 0) {
        return -1;
    }
    while (task_queue_is_empty(queue)) {
        // 队列为空，等待条件变量
        if(pthread_cond_wait(&queue->cond, &queue->mutex) != 0) {
            pthread_mutex_unlock(&queue->mutex);
            return -1;
        }
    }
    // 出队
    *task = queue->tasks[queue->front];
    queue->front = (queue->front + 1) & (queue->cap - 1);
    // 设置上次操作为出队
    queue->operat = QUEUE_DEQUEUE;
    // 解锁
    if(pthread_mutex_unlock(&queue->mutex) != 0) {
        return -1;
    }
    // 通知等待条件变量的线程
    if(pthread_cond_signal(&queue->cond) != 0) {
        return -1;
    }
    return 0;
}
