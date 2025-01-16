#ifndef AUTHENTICAT_H
#define AUTHENTICAT_H

struct state {
    char cwd[64];      // 当前工作目录
    char username[21]; // 用户名
    char password[19];  // 密码
};

/**
 * @brief 封装 sign_in 和 sign_up
 * 
 * @param fd 与服务端连接的套接字
 * @param state 存储客户端的一些状态信息
 * @return int 0: 登陆或注册成功; -1: 登陆或注册失败
 */
int sign(int fd, struct state *state);

#endif // AUTHENTICAT_H
