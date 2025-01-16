#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "tcp.h"
#include "authenticat.h"
#include "cmd.h"

#define HOSTNAME "127.0.0.1"
#define SERVICE "8080"

int main(int argc, char *argv[])
{
    if (argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
    {
        fprintf(stderr, "Usage: %s <hostname> <service>\n", argv[0]);
        fprintf(stderr, "   <hostname>\t\tThe hostname of the server.\n");
        fprintf(stderr, "   <service>\t\tThe service of the server.\n");
        return 0;
    }
    char *hostname = argc > 1 ? argv[1] : HOSTNAME;
    char *service = argc > 2 ? argv[2] : SERVICE;

    // 连接服务器
    int sockfd = tcp_connect(hostname, service);
    if (sockfd == -1)
    {
        fprintf(stderr, "Failed to connect to %s:%s\n", hostname, service);
        return 1;
    }
    struct state state; // 客户端状态
    // 登陆或注册
    if(sign(sockfd, &state) == -1) {
        fprintf(stderr, "Failed to sign in or sign up\n");
        return 1;
    }
    // 登陆成功：清除终端中的消息
    printf("\033[2J\033[1;1H");
    // 开启一个循环，等待用户输入命令：展示一个类似于 shell 的提示符
    while (true)
    {
        printf("\n%s@%s>", state.username, state.cwd);
        struct command command;
        // 读取用户输入的命令
        if(command_read(&command) == -1) {
            fprintf(stderr, "Failed to read command\n");
            return 1;
        }
        // 执行命令
        if(command_execute(sockfd, &command, &state) == -1) {
            fprintf(stderr, "Failed to execute command\n");
            return 1;
        }

    }

    return 0;
}
