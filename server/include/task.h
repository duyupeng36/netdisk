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


/**
 * @brief 执行任务
 * 
 * @param task 任务结构体指针, 指向需要执行的任务结构体
 * @return int 成功返回 0，失败返回 -1
 */
int execute_task(task_t *task);

/**
 * @brief 登陆服务器
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 登陆命令参数个数
 * @param argv 登陆命令参数 {login -u username -p [password]} 
 * @return int  成功登陆返回 0，失败返回 -1
 */
int task_login(int fd, int argc, char *argv[]);

/**
 * @brief 新用户注册
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 注册命令参数个数
 * @param argv 注册命令参数 {register -u [username] -p [password]}
 * @return int 成功注册返回 0，失败返回 -1
 */
int task_register(int fd, int argc, char *argv[]);

/**
 * @brief 创建目录
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 创建目录命令参数个数
 * @param argv 创建目录命令参数 {mkdir [dir]}
 * @return int 成功创建返回 0，失败返回 -1
 */
int task_mkdir(int fd, int argc, char *argv[]);

/**
 * @brief 删除目录
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 删除目录命令参数个数
 * @param argv 删除目录命令参数 {rmdir [dir]}
 * @return int 成功删除返回 0，失败返回 -1
 */
int task_rmdir(int fd, int argc, char *argv[]);

/**
 * @brief 切换目录
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 切换目录命令参数个数
 * @param argv 切换目录命令参数 {cd [dir]}
 * @return int 成功切换返回 0，失败返回 -1
 */
int task_cd(int fd, int argc, char *argv[]);

/**
 * @brief 显示当前目录
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 显示当前目录命令参数个数
 * @param argv 显示当前目录命令参数 {pwd}
 * @return int 成功显示返回 0，失败返回 -1
 */
int task_pwd(int fd, int argc, char *argv[]);

/**
 * @brief 列出文件
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 列出文件命令参数个数
 * @param argv 列出文件命令参数 {ls}
 * @return int 成功列出返回 0，失败返回 -1
 */
int task_ls(int fd, int argc, char *argv[]);

/**
 * @brief 下载文件
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 下载文件命令参数个数
 * @param argv 下载文件命令参数 {gets [file]}
 * @return int 成功下载返回 0，失败返回 -1
 */
int task_gets(int fd, int argc, char *argv[]);

/**
 * @brief 上传文件
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 上传文件命令参数个数
 * @param argv 上传文件命令参数 {puts [file]}
 * @return int 成功上传返回 0，失败返回 -1
 */
int task_puts(int fd, int argc, char *argv[]);

/**
 * @brief 删除文件
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 删除文件命令参数个数
 * @param argv 删除文件命令参数 {remove [file]}
 * @return int 成功删除返回 0，失败返回 -1
 */
int task_remove(int fd, int argc, char *argv[]);

/**
 * @brief 退出
 * 
 * @param fd 与客户端通信的套接字
 * @param argc 退出命令参数个数
 * @param argv 退出命令参数 {quit}
 * @return int 成功退出返回 0，失败返回 -1
 */
int task_quit(int fd, int argc, char *argv[]);

#endif // TASK_H