#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <shadow.h>
#include <crypt.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "task.h"
#include "cmd.h"
#include "tcp.h"
#include "message.h"
#include "connection.h"

#define PATH_PREFIX "/home/dyp/ftp"

#define MESSAGE_TEMPLATE "{\"type\": \"%s\", \"%s\": \"%s\", \"%s\": \"%s\"}"
#define MESSAGE_LENGTH 8192

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
    extern connect_table_t *connections;
    // 查找当前连接
    connection_t *conn = NULL;
    if(connect_table_find(connections, fd, &conn) != 0) {
        printf("find connection failed\n");
        return -1;
    }
    char message[MESSAGE_LENGTH] = {0};

    char *username = NULL;
    char *password = NULL;

    /* 参数解析 */
    struct optparse options;
    optparse_init(&options, argv);
    int option;
    while ((option = optparse(&options, "u:p:")) != -1)
    {
        switch (option)
        {
        case 'u':
            username = options.optarg;
            break;
        case 'p':
            password = options.optarg;
            break;
        case '?':
            snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "unknown option", "example", "login -u username -p password");
            // 发送消息
            message_send(fd, message);
            break;
        }
    }

    // 获取用户名对应的密码
    struct spwd *sp = getspnam(username);
    if(sp == NULL) {
        // 没有该用户
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "no such user", "example", "login -u username -p password");
        // 发送消息
        message_send(fd, message);
        return -1;
    }
    
    struct crypt_data data;
    char *encrypted = crypt_r(password, sp->sp_pwdp, &data);
    if(strcmp(encrypted, sp->sp_pwdp) != 0) {
        // 密码错误
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "password error", "example", "login -u username -p password");
        // 发送消息
        message_send(fd, message);
        return -1;
    }

    // 登陆成功
    // 设置连接的用户名
    strcpy(conn->username, username);
    // 设置连接的 token
    snprintf(conn->token, 128, "%s:%s", username, "123456");
    
    // 获取当前工作目录
    char cwd[2048] = {0};
    if(cwd_pwd(conn->cwd, cwd, 2048) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "get current working directory error", "example", "login -u username -p password");
        // 发送消息
        message_send(fd, message);
        return -1;
    }
    snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "success", "cwd", cwd, "token", username);
    // 发送消息
   if( message_send(fd, message) != 0) {
         return -1;
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
    extern connect_table_t *connections;
    // 查找当前连接
    connection_t *conn = NULL;
    if(connect_table_find(connections, fd, &conn) != 0) {
        printf("find connection failed\n");
        return -1;
    }
    char message[MESSAGE_LENGTH] = {0};
    // 获取当前工作目录
    char current[2048] = {0};
    if(cwd_pwd(conn->cwd, current, 2048) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "get current working directory error", "example", "mkdir [dir]");
        // 发送消息
        message_send(fd, message);
        return -1;
    }

    // 路径入栈
    if(cwd_push(conn->cwd, argv[1]) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "change directory failed", "example", "mkdir [dir]");
        // 发送消息
        message_send(fd, message);
        return -1;
    }

    // 获取当前工作目录
    char cwd[2048] = {0};
    if(cwd_pwd(conn->cwd, cwd, 2048) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "get current working directory error", "example", "mkdir [dir]");
        // 发送消息
        message_send(fd, message);
        return -1;
    }
    // 拼接真实路径
    char real_path[4096] = {0};
    snprintf(real_path, 4096, "%s/%s%s", PATH_PREFIX, conn->username, cwd);
    printf("real path: %s\n", real_path);

    // 不递归
    if(mkdir(real_path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IRGRP) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "create directory failed", "example", "mkdir [-p] dir");
        // 发送消息
        message_send(fd, message);
        // 切换回原来的目录
        cwd_push(conn->cwd, current);
        return -1;
    }
    // 切换回原来的目录
    cwd_push(conn->cwd, current);
    snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "success", "cwd", current, "example", "mkdir [-p] dir");
    // 发送消息
    message_send(fd, message);
    return 0;
}

int task_rmdir(int fd, int argc, char *argv[])
{
    extern connect_table_t *connections;
    // 查找当前连接
    connection_t *conn = NULL;
    if(connect_table_find(connections, fd, &conn) != 0) {
        printf("find connection failed\n");
        return -1;
    }
    char message[MESSAGE_LENGTH] = {0};
    // 获取当前工作目录
    char current[2048] = {0};
    if(cwd_pwd(conn->cwd, current, 2048) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "get current working directory error", "example", "rmdir [dir]");
        // 发送消息
        message_send(fd, message);
        return -1;
    }
    // 路径入栈
    if(cwd_push(conn->cwd, argv[1]) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "change directory failed", "example", "rmdir [dir]");
        // 发送消息
        message_send(fd, message);
        return -1;
    }

    // 获取当前工作目录
    char cwd[2048] = {0};
    if(cwd_pwd(conn->cwd, cwd, 2048) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "get current working directory error", "example", "rmdir [dir]");
        // 发送消息
        message_send(fd, message);
        return -1;
    }
    // 拼接真实路径
    char real_path[4096] = {0};
    snprintf(real_path, 4096, "%s/%s%s", PATH_PREFIX, conn->username, cwd);
    printf("real path: %s\n", real_path);
    // 检查该目录是否存在
    if(access(real_path, F_OK) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "no such directory", "example", "rmdir [dir]");
        // 发送消息
        message_send(fd, message);
        // 切换回原来的目录
        cwd_push(conn->cwd, current);
        return -1;
    }
    // 调用 rmdir 函数删除目录
    if(rmdir(real_path) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "remove directory failed", "example", "rmdir [dir]");
        // 发送消息
        message_send(fd, message);
        // 切换回原来的目录
        cwd_push(conn->cwd, current);
        return -1;
    }
    // 切换回原来的目录
    cwd_push(conn->cwd, current);
    snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "success", "cwd", current, "example", "rmdir [dir]");
    // 发送消息
    message_send(fd, message);
    return 0;
}


int task_cd(int fd, int argc, char *argv[])
{
    char message[MESSAGE_LENGTH] = {0};
    if(argc != 2) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "missing argument", "example", "cd [dir]");
        // 发送消息
        message_send(fd, message);
        return -1;
    }
    extern connect_table_t *connections;
    // 查找当前连接
    connection_t *conn = NULL;
    if(connect_table_find(connections, fd, &conn) != 0) {
        printf("find connection failed\n");
        return -1;
    }

    char current[2048] = {0};
    if(cwd_pwd(conn->cwd, current, 2048) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "get current working directory error", "example", "cd [dir]");
        // 发送消息
        message_send(fd, message);
        return -1;
    }

    // 切换目录
    if(cwd_push(conn->cwd, argv[1]) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "change directory failed", "example", "cd [dir]");
        // 发送消息
        message_send(fd, message);
        return -1;
    }

    // 获取当前工作目录
    char cwd[2048] = {0};
    if(cwd_pwd(conn->cwd, cwd, 2048) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "get current working directory error", "example", "cd [dir]");
        // 发送消息
        message_send(fd, message);
        return -1;
    }
    // 拼接真实路径
    char real_path[4096] = {0};
    snprintf(real_path, 4096, "%s/%s%s", PATH_PREFIX, conn->username, cwd);
    printf("real path: %s\n", real_path);
    // 检查该目录是否存在
    if(access(real_path, F_OK) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "no such directory", "example", "cd [dir]");
        // 发送消息
        message_send(fd, message);
        // 切换回原来的目录
        cwd_push(conn->cwd, current);
        return -1;
    }
    snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "success", "cwd", cwd, "example", "cd [dir]");
    // 发送消息
    message_send(fd, message);
    return 0;
}

int task_pwd(int fd, int argc, char *argv[])
{
    extern connect_table_t *connections;
    // 查找当前连接
    connection_t *conn = NULL;
    if(connect_table_find(connections, fd, &conn) != 0) {
        printf("find connection failed\n");
        return -1;
    }
    char message[MESSAGE_LENGTH] = {0};
    // 获取当前工作目录
    char cwd[2048] = {0};
    if(cwd_pwd(conn->cwd, cwd, 2048) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "get current working directory error", "example", "pwd");
        // 发送消息
        message_send(fd, message);
        return -1;
    }
    snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "success", "cwd", cwd, "example", "pwd");
    printf("message: %s\n", message);
    // 发送消息
    message_send(fd, message);
    return 0;
}

int task_ls(int fd, int argc, char *argv[])
{
    extern connect_table_t *connections;
    // 查找当前连接
    connection_t *conn = NULL;
    if(connect_table_find(connections, fd, &conn) != 0) {
        printf("find connection failed\n");
        return -1;
    }
    char message[MESSAGE_LENGTH] = {0};
    // 获取当前工作目录
    char current[2048] = {0};
    if(cwd_pwd(conn->cwd, current, 2048) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "get current working directory error", "example", "ls");
        // 发送消息
        message_send(fd, message);
        return -1;
    }
    if(argc == 2) {
        // 路径入栈
        if(cwd_push(conn->cwd, argv[1]) != 0) {
            snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "change directory failed", "example", "ls");
            // 发送消息
            message_send(fd, message);
            return -1;
        }
    }
    // 获取当前工作目录
    char cwd[2048] = {0};
    if(cwd_pwd(conn->cwd, cwd, 2048) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "get current working directory error", "example", "ls");
        // 发送消息
        message_send(fd, message);
        return -1;
    }
    // 拼接真实路径
    char real_path[4096] = {0};
    snprintf(real_path, 4096, "%s/%s%s", PATH_PREFIX, conn->username, cwd);
    printf("real path: %s\n", real_path);
    // 检查该目录是否存在
    if(access(real_path, F_OK) != 0) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "no such directory", "example", "ls");
        // 发送消息
        message_send(fd, message);
        // 切换回原来的目录
        cwd_push(conn->cwd, current);
        return -1;
    }
    // 打开目录
    DIR *dir = opendir(real_path);
    if(dir == NULL) {
        snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "error", "message", "open directory failed", "example", "ls");
        // 发送消息
        message_send(fd, message);
        // 切换回原来的目录
        cwd_push(conn->cwd, current);
        return -1;
    }
    // 读取目录
    char all_files[4096] = {0};
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL) {
        strncat(all_files, entry->d_name, strlen(entry->d_name));
        strcat(all_files, " ");
    }
    closedir(dir);
    snprintf(message, MESSAGE_LENGTH, MESSAGE_TEMPLATE, "success", "files", all_files, "example", "ls");
    printf("message: %s\n", message);
    // 发送消息
    message_send(fd, message);

    // 切换回原来的目录
    cwd_push(conn->cwd, current);

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
