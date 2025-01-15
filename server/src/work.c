
#include <stdio.h>
#include "work.h"

/**
 * @brief 登陆服务器
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 登陆命令参数个数
 * @param argv 登陆命令参数 {login -u username -p [password]} 
 * @return int  成功登陆返回 0，失败返回 -1
 */
static int task_login(int fd, int argc, char *argv[]);

/**
 * @brief 新用户注册
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 注册命令参数个数
 * @param argv 注册命令参数 {register -u [username] -p [password]}
 * @return int 成功注册返回 0，失败返回 -1
 */
static int task_register(int fd, int argc, char *argv[]);

/**
 * @brief 创建目录
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 创建目录命令参数个数
 * @param argv 创建目录命令参数 {mkdir [dir]}
 * @return int 成功创建返回 0，失败返回 -1
 */
static int task_mkdir(int fd, int argc, char *argv[]);

/**
 * @brief 删除目录
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 删除目录命令参数个数
 * @param argv 删除目录命令参数 {rmdir [dir]}
 * @return int 成功删除返回 0，失败返回 -1
 */
static int task_rmdir(int fd, int argc, char *argv[]);

/**
 * @brief 切换目录
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 切换目录命令参数个数
 * @param argv 切换目录命令参数 {cd [dir]}
 * @return int 成功切换返回 0，失败返回 -1
 */
static int task_cd(int fd, int argc, char *argv[]);

/**
 * @brief 显示当前目录
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 显示当前目录命令参数个数
 * @param argv 显示当前目录命令参数 {pwd}
 * @return int 成功显示返回 0，失败返回 -1
 */
static int task_pwd(int fd, int argc, char *argv[]);

/**
 * @brief 列出文件
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 列出文件命令参数个数
 * @param argv 列出文件命令参数 {ls}
 * @return int 成功列出返回 0，失败返回 -1
 */
static int task_ls(int fd, int argc, char *argv[]);

/**
 * @brief 下载文件
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 下载文件命令参数个数
 * @param argv 下载文件命令参数 {gets [file]}
 * @return int 成功下载返回 0，失败返回 -1
 */
static int task_gets(int fd, int argc, char *argv[]);

/**
 * @brief 上传文件
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 上传文件命令参数个数
 * @param argv 上传文件命令参数 {puts [file]}
 * @return int 成功上传返回 0，失败返回 -1
 */
static int task_puts(int fd, int argc, char *argv[]);

/**
 * @brief 删除文件
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 删除文件命令参数个数
 * @param argv 删除文件命令参数 {remove [file]}
 * @return int 成功删除返回 0，失败返回 -1
 */
static int task_remove(int fd, int argc, char *argv[]);

/**
 * @brief 退出
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 退出命令参数个数
 * @param argv 退出命令参数 {quit}
 * @return int 成功退出返回 0，失败返回 -1
 */
static int task_quit(int fd, int argc, char *argv[]);


int execute_task(task_t *task) {
    switch (task->type)
    {
    case TASK_LOGIN:        // 登陆任务
        return task_login(task->fd, task->argc, task->argv);
    case TASK_REGISTER:     // 注册任务
        return task_register(task->fd, task->argc, task->argv);
    case TASK_MKDIR:        // 创建目录任务
        return task_mkdir(task->fd, task->argc, task->argv);
    case TASK_RMDIR:        // 删除目录任务
        return task_rmdir(task->fd, task->argc, task->argv);
    case TASK_CD:           // 切换目录任务
        return task_cd(task->fd, task->argc, task->argv);
    case TASK_PWD:          // 显示当前目录任务
        return task_pwd(task->fd, task->argc, task->argv);
    case TASK_LS:           // 列出文件任务
        return task_ls(task->fd, task->argc, task->argv);
    case TASK_GETS:         // 下载文件任务
        return task_gets(task->fd, task->argc, task->argv);
    case TASK_PUTS:         // 上传文件任务
        return task_puts(task->fd, task->argc, task->argv);
    case TASK_REMOVE:       // 删除文件任务
        return task_remove(task->fd, task->argc, task->argv);
    case TASK_QUIT:         // 退出任务
        return task_quit(task->fd, task->argc, task->argv);
    default:
        return -1;
    }    
    return 0;
}

int task_login(int fd, int argc, char *argv[]) {
    printf("login\n");
    return 0;
}

int task_register(int fd, int argc, char *argv[]) {
    printf("register\n");
    return 0;
}

int task_mkdir(int fd, int argc, char *argv[]) {
    printf("mkdir\n");
    return 0;
}

int task_rmdir(int fd, int argc, char *argv[]) {
    printf("rmdir\n");
    return 0;
}

int task_cd(int fd, int argc, char *argv[]) {
    printf("cd\n");
    return 0;
}

int task_pwd(int fd, int argc, char *argv[]) {
    printf("pwd\n");
    return 0;
}

int task_ls(int fd, int argc, char *argv[]) {
    printf("ls\n");
    return 0;
}

int task_gets(int fd, int argc, char *argv[]) {
    printf("gets\n");
    return 0;
}

int task_puts(int fd, int argc, char *argv[]) {
    printf("puts\n");
    return 0;
}

int task_remove(int fd, int argc, char *argv[]) {
    printf("remove\n");
    return 0;
}

int task_quit(int fd, int argc, char *argv[]) {
    printf("quit\n");
    return 0;
}

