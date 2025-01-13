#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "task.h"


/**
 * @brief create a task, and return the pointer to the task
 * 
 * @param fd the file descriptor of the network communication
 * @param argc the number of arguments
 * @param argv the arguments. argv[0] is the command, and the others are the arguments
 * @param handler the task handler function
 * @return task_t* the pointer to the task，on error, return NULL
 */
static task_t * task_create(user_t *user, int argc, char *argv[], task_handler_t handler);

/**
 * @brief destroy a task
 * 
 * @param t the task to be destroyed
 */
static void task_destroy(task_t * t);


int task_queue_init(task_queue_t * queue) {
    if(queue == NULL) {
        return -1;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    // 初始化互斥锁
    if(pthread_mutex_init(&queue->mutex, NULL) != 0) {
        return -1;
    }
    // 初始化条件变量
    if(pthread_cond_init(&queue->cond, NULL) != 0) {
        return -1;
    }
    return 0;
}

int task_queue_push(task_queue_t * queue, user_t *user, int argc, char *argv[], task_handler_t handler) {
    if(queue == NULL) {
        return -1;
    }
    task_t *t = task_create(user, argc, argv, handler);
    if(t == NULL) {
        return -1;
    }

    // 加锁: 保护队列的操作
    if(pthread_mutex_lock(&queue->mutex) != 0) {
        return -1;
    }

    // 队列满了，等待子线程处理任务
    while (queue->size == MAX_TASK_QUEUE_SIZE)
    {
        // 等待条件变量发生变化
        if(pthread_cond_wait(&queue->cond, &queue->mutex) != 0) {
            return -1;
        }
    }
    
    if(queue->head == NULL) {
        queue->head = t;
        queue->tail = t;
    } else {
        queue->tail->next = t;
        queue->tail = t;
    }
    queue->size++;

    // 解锁：释放锁
    if(pthread_mutex_unlock(&queue->mutex) != 0) {
        return -1;
    }

    // 通知等待的线程条件变量发生变化
    if(pthread_cond_signal(&queue->cond) != 0) {
        return -1;
    }
    
    return 0;
}

int task_queue_pop(task_queue_t * queue, task_t * t) {
    if(queue == NULL || t == NULL) {
        return -1;
    }
    // 加锁: 保护队列的操作
    if(pthread_mutex_lock(&queue->mutex) != 0) {
        return -1;
    }
    // 队列为空，等待主线程将任务添加到队列
    while (queue->size == 0) 
    {
        // 等待条件变量发生变化
        if(pthread_cond_wait(&queue->cond, &queue->mutex) != 0) {
            return -1;
        }
    }
    // 从队列中取出任务
    *t = *(queue->head);
    
    // 释放队列中被取出的任务
    task_t *tmp = queue->head;
    queue->head = queue->head->next;
    
    task_destroy(tmp);
    queue->size--;
    
    // 队列为空
    if(queue->size == 0) {
        queue->tail = NULL;
    }
    // 解锁：释放锁
    if(pthread_mutex_unlock(&queue->mutex) != 0) {
        return -1;
    }
    // 通知主线程条件变量发生变化
    if(pthread_cond_signal(&queue->cond) != 0) {
        return -1;
    }
    return 0;
}

int task_queue_destroy(task_queue_t * queue) {
    if (queue == NULL) {
        return -1;
    }
    // 释放任务队列中的未被执行的任务任务
    while (queue->head != NULL)
    {
        task_t *tmp = queue->head;
        queue->head = queue->head->next;
        // 释放任务
        task_destroy(tmp);
    }

    // 销毁互斥锁
    if(pthread_mutex_destroy(&queue->mutex) != 0) {
        return -1;
    }

    // 销毁条件变量
    if(pthread_cond_destroy(&queue->cond) != 0) {
        return -1;
    }
    return 0;
}


task_t * task_create(user_t *user, int argc, char *argv[], task_handler_t handler) {
    task_t *t = (task_t *)malloc(sizeof(task_t));
    if(t == NULL) {
        return NULL;
    }
    t->user = user;
    t->argc = argc;

    // 为参数列表分配内存
    t->argv = (char **)malloc(sizeof(char *) * argc + 1);
    if(t->argv == NULL) {
        free(t);
        return NULL;
    }
    memcpy(t->argv, argv, sizeof(char *) * argc + 1);
    t->handler = handler;
    t->next = NULL;
    return t;
}

void task_destroy(task_t * t) {
    if(t == NULL) {
        return;
    }
    // 释放参数列表
    free(t->argv);
    // 释放任务
    free(t);
}
