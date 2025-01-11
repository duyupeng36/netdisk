#ifndef TASK_H
#define TASK_H

#include <pthread.h>
#include <stdint.h>

#define MAX_TASK_QUEUE_SIZE 1024

typedef struct task task_t;

/**
 * @brief task handler function
 * 
 * @param fd the file descriptor of the network communication
 * @return on success, return 0; on error, return -1
 */
typedef int(*task_handler_t)(int fd);

/**
 * @brief task queue structure
 * 
 * + head: the head of the task queue
 * 
 * + tail: the tail of the task queue
 * 
 * + mutex: the mutex lock for the task queue
 * 
 * + cond: the condition variable for the task queue
 * 
 */
typedef struct task_queue {
    task_t *head;      // 任务队列的头
    task_t *tail;      // 任务队列的尾
    uint64_t size;       // 任务队列的大小
    pthread_mutex_t mutex;  // 互斥锁
    pthread_cond_t cond;    // 条件变量
} task_queue_t;

struct task {
    int fd;                 // 网络通信的文件描述符
    task_handler_t handler; // 任务处理函数
    task_t *next;           // 下一个任务
};

/**
 * @brief initiate a empty task queue
 * 
 * @param queue the task queue to be initiated
 * @return on success, return 0; on error, return -1
 */
int task_queue_init(task_queue_t * queue);

/**
 * @brief add a task to the task queue
 * 
 * @param queue the task queue, where the task will be added
 * @param fd the file descriptor of the network communication
 * @param handler the task handler function
 * @return on success, return 0; on error, return -1
 */
int task_queue_push(task_queue_t * queue, int fd, task_handler_t handler);

/**
 * @brief pop a task from the task queue
 * 
 * @param queue the task queue, where the task will be popped
 * @param t the task to be popped
 * @return on success, return 0; on error, return -1
 */
int task_queue_pop(task_queue_t * queue, task_t * t);

/**
 * @brief Destroy task queue
 * 
 * @param queue the task queue to be destroyed
 * @return on success, return 0; on error, return -1
 */
int task_queue_destroy(task_queue_t * queue);

#endif // TASK_H
