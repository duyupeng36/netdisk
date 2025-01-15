#ifndef WORK_H
#define WORK_H


#include "task.h"

/**
 * @brief 执行任务
 * 
 * @param task 待执行的任务
 * @return int 任务是否执行成功
 */
int execute_task(task_t *task);

#endif // WORK_H
