#ifndef AUTHENTICAT_H
#define AUTHENTICAT_H

#include "state.h"

/**
 * @brief 封装 sign_in 和 sign_up
 * 
 * @param fd 与服务端连接的套接字
 * @param state 存储客户端的一些状态信息
 * @return int 0: 登陆或注册成功; -1: 登陆或注册失败
 */
int sign(int fd, struct state *state);

#endif // AUTHENTICAT_H
