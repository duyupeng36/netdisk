#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>

#define _XOPEN_SOURCE
#include <unistd.h>

#include "cmd.h"
#include "tcp.h"
#include "authenticat.h"


/**
 * @brief 登陆服务器
 * 
 * @param fd 与服务端连接的套接字
 * @param state 存储客户端的一些状态信息
 * @return int 0: 登陆成功; -1: 登陆失败
 */
static int sign_in(int fd, struct state * state);

/**
 * @brief 新用户注册
 * 
 * @param fd 与服务端连接的套接字
 * @param state 存储客户端的一些状态信息
 * @return int 0: 注册成功; -1: 注册失败
 */
static int sign_up(int fd, struct state * state);

int sign(int fd, struct state *state)
{
    bool success;
    while (true)
    {
        int choice;
        success = false;
        printf("1. Sign in\n2. Sign up\n");
        printf("Please enter your choice: ");
        scanf("%d%*c", &choice);
        switch (choice)
        {
        case 1:
            success = sign_in(fd, state) == 0;
            break;
        case 2:
            success = sign_up(fd, state) == 0;
            break;
        default:
            printf("Invalid choice\n");
            break;
        }
        if (success)
        {
            break;
        }
    }
    return success ? 0 : -1;
}

/**
 * @brief 发送密码给服务器
 * 
 * @param fd 与服务端连接的套接字
 * @param password 密码字符串
 * @return int 0: 发送成功; -1: 发送失败
 */
static int password_send(int fd, const char *password) {
    int password_length = strlen(password);
    if(sendn(fd, &password_length, sizeof(int)) != sizeof(int)) {
        return -1;
    }
    if(sendn(fd, password, password_length) != password_length) {
        return -1;
    }
    return 0;
}

/**
 * @brief 接收 cwd
 * 
 * @param fd 与服务端连接的套接字
 * @param state 存储客户端的一些状态信息
 * @return int 0: 接收成功; -1: 接收失败
 */
static int cwd_recv(int fd, struct state *state) {
    // 接收 cwd 的长度
    int cwd_length;
    if(recvn(fd, &cwd_length, sizeof(int)) != sizeof(int)) {
        return -1;
    }
    // 接收 cwd
    state->cwd = malloc(sizeof(char) * (cwd_length + 1));
    if(state->cwd == NULL) {
        return -1;
    }
    if(recvn(fd, state->cwd, cwd_length) != cwd_length) {
        return -1;
    }
    state->cwd[cwd_length] = '\0';
    return 0;
}

/**
 * @brief 接收服务端颁发的令牌
 * 
 * @param fd 与服务端连接的套接字
 * @param state 存储客户端的一些状态信息
 * @return int 0: 接收成功; -1: 接收失败
 */
static int token_recv(int fd, struct state *state) {
    // 接收 token 的长度
    int token_length;
    if(recvn(fd, &token_length, sizeof(int)) != sizeof(int)) {
        return -1;
    }
    // 接收 token
    state->token = malloc(sizeof(char) * (token_length + 1));
    if(state->token == NULL) {
        return -1;
    }
    if(recvn(fd, state->token, token_length) != token_length) {
        return -1;
    }
    state->token[token_length] = '\0';
    return 0;
}

/**
 * @brief 输入密码
 * 
 * @param password 
 * @param n 
 * @return int 
 */
static int getpass(const char *prompt, char *password, size_t n) {

    struct termios oldt, newt;
    // 获取终端设置
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    // 关闭回显
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("%s", prompt);
    fgets(password, n-1, stdin);
    // 恢复终端设置
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    password[strlen(password)] = '\0';
    return 0;
}

int sign_in(int fd, struct state *state)
{
    char username[1024] = {0};   // 用户名
    printf("Please enter your username: ");
    fgets(username, 1024, stdin);
    char password[1024] = {0}; // 密码
    getpass("Please enter your password: ", password, 1024);

    state->username = malloc(strlen(username) + 1);
    strncpy(state->username, username, strlen(username));
    state->username[strlen(username)] = '\0';

    // 发起登陆请求: login  -u <username> -p <password> 
    char command_buffer[4096] = {0};
    snprintf(command_buffer, 4096, "login -u%s -p %s", username, password);
    struct command command;
    command.command = command_buffer;
    command.command_length = strlen(command_buffer);
    if(command_send(fd, &command) == -1) {
        return -1;
    }
    // 等待服务端响应的 cwd 和 token
    // 接收 cwd 的长度
    if(cwd_recv(fd, state) == -1) {
        return -1;
    }

    // 接收 token 的长度
    if(token_recv(fd, state) == -1) {
        return -1;
    }
    return 0;
}

int sign_up(int fd, struct state *state)
{
    return 0;
}
