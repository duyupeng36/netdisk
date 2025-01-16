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
    fgets(password, n, stdin);
    // 恢复终端设置
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    password[strlen(password)-1] = '\0';
    return 0;
}

int sign_in(int fd, struct state *state)
{
    printf("Please enter your username: ");
    fgets(state->username, sizeof(state->username), stdin);
    state->username[strlen(state->username) - 1] = '\0'; // 去掉换行符
    getpass("Please enter your password: ", state->password, sizeof(state->password));

    // 发起登陆请求: login  -u <username> -p <password> 
    char command_buffer[1024] = {0};
    snprintf(command_buffer, sizeof(command_buffer), "login -u%s -p %s", state->username, state->password);
    struct command command;
    command.command = command_buffer;
    command.command_length = strlen(command_buffer);
    if(command_send(fd, &command) == -1) {
        return -1;
    }

    strncpy(state->cwd, "/", 64);
    return 0;
}

int sign_up(int fd, struct state *state)
{
    return 0;
}
