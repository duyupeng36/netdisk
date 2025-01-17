#ifndef STATE_H

#define STATE_H

struct state {
    char cwd[2048];      // 当前工作目录
    char username[21]; // 用户名
    char password[19];  // 密码
    char token[128];    // 令牌
};

#endif // STATE_H
