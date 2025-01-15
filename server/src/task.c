#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "cmd.h"


task_t *parse_task(int fd, const char *command)
{
    task_t *task = (task_t *)malloc(sizeof(task_t));
    if (task == NULL)
    {
        return NULL;
    }
    task->fd = fd;
    task->argv = parse_command(command, &task->argc);
    if (task->argv == NULL)
    {
        free(task);
        return NULL;
    }
    // 根据命令设置任务类型
    if (strcmp(task->argv[0], "login") == 0)
    {
        task->type = TASK_LOGIN;
    }
    else if (strcmp(task->argv[0], "register") == 0)
    {
        task->type = TASK_REGISTER;
    }
    else if (strcmp(task->argv[0], "mkdir") == 0)
    {
        task->type = TASK_MKDIR;
    }
    else if (strcmp(task->argv[0], "rmdir") == 0)
    {
        task->type = TASK_RMDIR;
    }
    else if (strcmp(task->argv[0], "cd") == 0)
    {
        task->type = TASK_CD;
    }
    else if (strcmp(task->argv[0], "pwd") == 0)
    {
        task->type = TASK_PWD;
    }
    else if (strcmp(task->argv[0], "ls") == 0)
    {
        task->type = TASK_LS;
    }
    else if (strcmp(task->argv[0], "gets") == 0)
    {
        task->type = TASK_GETS;
    }
    else if (strcmp(task->argv[0], "puts") == 0)
    {
        task->type = TASK_PUTS;
    }
    else if (strcmp(task->argv[0], "remove") == 0)
    {
        task->type = TASK_REMOVE;
    }
    else if (strcmp(task->argv[0], "quit") == 0)
    {
        task->type = TASK_QUIT;
    }
    else
    {
        // 命令无法识别
        free_task(task);
        return NULL;
    }
    
    return task;
}

void free_task(task_t *task)
{
    if (task == NULL)
    {
        return;
    }
    // 释放每个参数
    free_argv(task->argv);
    // 释放参数数组
    free(task->argv);
    // 释放任务结构体
    free(task);
}
