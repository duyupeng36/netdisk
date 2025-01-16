#ifndef RESPONSE_H
#define RESPONSE_H

#include "state.h"

/**
 * @brief 处理 mkdir 命令的响应
 * 
 * @param fd 
 * @param state 
 * @return int 
 */
int response_mkdir(int fd, struct state *state);

/**
 * @brief 处理 rmdir 命令的响应
 * 
 * @param fd 
 * @param state 
 * @return int 
 */
int response_rmdir(int fd, struct state *state);

/**
 * @brief 处理 ls 命令的响应
 * 
 * @param fd 
 * @param state 
 * @return int 
 */
int response_ls(int fd, struct state *state);

/**
 * @brief 处理 cd 命令的响应
 * 
 * @param fd 
 * @param state 
 * @return int 
 */
int response_cd(int fd, struct state *state);

/**
 * @brief 处理 pwd 命令的响应
 * 
 * @param fd 
 * @param state 
 * @return int 
 */
int response_pwd(int fd, struct state *state);

/**
 * @brief 处理 gets 命令的响应
 * 
 * @param fd 
 * @param state 
 * @return int 
 */
int response_gets(int fd, struct state *state);

/**
 * @brief 处理 puts 命令的响应
 * 
 * @param fd 
 * @param state 
 * @return int 
 */
int response_puts(int fd, struct state *state);

/**
 * @brief 处理 remove 命令的响应
 * 
 * @param fd 
 * @param state 
 * @return int 
 */
int response_remove(int fd, struct state *state);

#endif // RESPONSE_H
