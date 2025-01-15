#ifndef TASK_H
#define TASK_H

typedef enum task_type
{
    /* 用户的登陆和注册任务 */
    TASK_LOGIN,     // 登陆任务
    TASK_REGISTER,  // 注册任务
    
    /* 目录操作任务 */
    TASK_MKDIR,     // 创建目录任务
    TASK_RMDIR,     // 删除目录任务
    TASK_CD,        // 切换目录任务
    TASK_PWD,       // 显示当前目录任务
    TASK_LS,        // 列出文件任务

    /* 文件操作任务 */
    TASK_GETS,      // 下载文件任务
    TASK_PUTS,      // 上传文件任务
    TASK_REMOVE,    // 删除文件任务

    /* 服务端退出任务 */
    TASK_QUIT, // 退出任务
} task_type_t;

typedef struct task {
    task_type_t type;   // 任务类型
    int fd;             // 任务对应的连接套接字
    int argc;           // 任务参数个数
    char **argv;        // 任务参数列表
} task_t;

/**
 * @brief 从 command 中解析任务
 * 
 * @param fd 任务对应的连接套接字
 * @param command 命令字符串
 * @return task_t* 成功返回任务结构体指针，失败返回 NULL
 */
task_t *parse_task(int fd, const char *command);


/**
 * @brief 释放任务结构体
 * 
 * @param task 任务结构体指针, 指向需要释放的任务结构体
 */
void free_task(task_t *task);


#endif // TASK_H