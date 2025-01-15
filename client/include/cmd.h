#ifndef CMD_H

#include "authenticat.h"

typedef struct command {
    int command_length;
    char *command;
} command_t;

/**
 * @brief 从标准输入中读取命令字符串
 * 
 * @param command 命令结构
 * @return int 0: 读取成功; -1: 读取失败
 */
int command_read(command_t *command);

/**
 * @brief 释放命令
 * 
 * @param command 命令结构
 */
void command_free(command_t *command);

/**
 * @brief 发送命令字符串到服务端
 * 
 * @param fd 与服务端连接的套接字
 * @param command 命令结构
 * @return int 0: 发送成功; -1: 发送失败
 */
int command_send(int fd, command_t *command);

/**
 * @brief 执行命令
 * 
 * @param fd 与服务端连接的套接字
 * @param command 待执行的命令
 * @param state 命令对客户端状态
 * @return int 0: 执行成功; -1: 执行失败
 */
int command_execute(int fd, command_t *command, struct state *state);

#endif // CMD_H


