#ifndef QUEUE_H
#define QUEUE_H
#include <pthread.h>
#include <stdbool.h>

#include "task.h"

typedef enum {
    QUEUE_ENQUEUE, // 入队
    QUEUE_DEQUEUE, // 出队
} operat_t;

typedef struct queue {
    task_t **tasks;         // 任务队列
    int rear;               // 队尾
    int front;              // 队头
    int cap;                // 队列大小
    operat_t operat;        // 上次操作的类型：入队或出队，初始值为出队
    pthread_mutex_t mutex;  // 互斥锁
    pthread_cond_t cond;    // 条件变量
} task_queue_t;


/**
 * @brief 创建任务队列
 * 
 * @param n 队列大小，取值为 2 ^ n
 * @return queue_t* 成功返回队列指针，失败返回 NULL
 */
task_queue_t *task_queue_crate(int n);

/**
 * @brief 销毁任务队列
 * 
 * @param queue 
 */
void task_queue_destroy(task_queue_t *queue);

/**
 * @brief 检查队列是否为空
 * 
 * @param queue 任务队列指针
 * @return int 队列为空返回 1，否则返回 0
 */
bool task_queue_is_empty(task_queue_t *queue);

/**
 * @brief 判断任务队列是否已满
 * 
 * @param queue 任务队列指针
 * @return int 队列已满返回 1，否则返回 0
 */
bool task_queue_is_full(task_queue_t *queue);

/**
 * @brief 将任务 task 入队
 * 
 * @param queue 任务队列 
 * @param task 任务，要入队的任务
 * @return int 成功返回 0，失败返回 -1
 */
int task_queue_enqueue(task_queue_t *queue, task_t *task);

/**
 * @brief 从队列中取出任务，并将其保存到 task 指向的指针中
 * 
 * @param queue 任务队列
 * @param task 任务指针，指向保存任务的指针
 * @return int 成功返回 0，失败返回 -1
 */
int task_queue_dequeue(task_queue_t *queue, task_t **task);

#endif // QUEUE_H

