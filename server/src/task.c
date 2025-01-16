#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "task.h"
#include "cmd.h"

#define OPTPARSE_IMPLEMENTATION
#include "optparse.h"

task_t *parse_task(int fd, const char *command)
{
    task_t *task = (task_t *)malloc(sizeof(task_t));
    if (task == NULL)
    {
        return NULL;
    }
    task->fd = fd;
    task->argv = command_parse(command, &task->argc);
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
    // 释放任务结构体
    free(task);
}

int execute_task(task_t *task)
{
    switch (task->type)
    {
    case TASK_LOGIN: // 登陆任务
        return task_login(task->fd, task->argc, task->argv);
    case TASK_REGISTER: // 注册任务
        return task_register(task->fd, task->argc, task->argv);
    case TASK_MKDIR: // 创建目录任务
        return task_mkdir(task->fd, task->argc, task->argv);
    case TASK_RMDIR: // 删除目录任务
        return task_rmdir(task->fd, task->argc, task->argv);
    case TASK_CD: // 切换目录任务
        return task_cd(task->fd, task->argc, task->argv);
    case TASK_PWD: // 显示当前目录任务
        return task_pwd(task->fd, task->argc, task->argv);
    case TASK_LS: // 列出文件任务
        return task_ls(task->fd, task->argc, task->argv);
    case TASK_GETS: // 下载文件任务
        return task_gets(task->fd, task->argc, task->argv);
    case TASK_PUTS: // 上传文件任务
        return task_puts(task->fd, task->argc, task->argv);
    case TASK_REMOVE: // 删除文件任务
        return task_remove(task->fd, task->argc, task->argv);
    case TASK_QUIT: // 退出任务
        return task_quit(task->fd, task->argc, task->argv);
    default:
        return -1;
    }
    return 0;
}

int task_login(int fd, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}

int task_register(int fd, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}

int task_mkdir(int fd, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}

int task_rmdir(int fd, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}

int task_cd(int fd, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}

int task_pwd(int fd, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}

int task_ls(int fd, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}

int task_gets(int fd, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}

int task_puts(int fd, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}

int task_remove(int fd, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}

int task_quit(int fd, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}
