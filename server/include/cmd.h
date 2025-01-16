#ifndef CMD_H
#define CMD_H


/**
 * @brief 返回一个 malloc 分配的字符串，包含从 fd 中读取的命令
 * 
 * @param fd 文件描述符
 * @return char* 成功返回命令字符串，失败返回 NULL
 */
const char *command_recv(int fd);

/**
 * @brief 释放 command 字符串
 * 
 * @param command 命令字符串，必须是通过 command_recv 函数获取的
 */
void command_free(const char *command);

/**
 * @brief 从 command 字符串中解析出命令及命令的参数。所有的参数都是 malloc 分配的字符串
 * 
 * @param command 命令字符串
 * @param argc 参数个数
 * @return char**  成功返回参数数组，由 malloc 分配；失败返回 NULL
 */
char **command_parse(const char *command, int *argc);

/**
 * @brief 释放参数数组
 * 
 * @param argv 参数数组，必须是通过 command_parse 函数获取的参数数组
 */
void free_argv(char **argv);
#endif // CMD_H