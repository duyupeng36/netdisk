#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <signal.h>

#include "tcp.h"
#include "cmd.h"

#define HOSTNAME "127.0.0.1"
#define SERVICE "8080"


int main(int argc, char *argv[]) {

    // 忽略 SIGINT 信号
    signal(SIGINT, SIG_IGN);
    // 忽略 SIGQUIT 信号
    signal(SIGQUIT, SIG_IGN);

    if(argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        printf("Usage: %s <hostname> <service>\n", argv[0]);
        printf("        - hostname: Host name of server\n");
        printf("        - service: Port number of the server\n");
        printf("Example: %s %s %s\n", argv[0], HOSTNAME, SERVICE);
        return 0;
    }

    const char *hostname = argc > 1 ? argv[1] : HOSTNAME;
    const char *service = argc > 2 ? argv[2] : SERVICE;

    // 与服务端建立连接
    int sockfd = tcp_connect(hostname, service);
    if(sockfd == -1) {
        // 连接失败
        perror("tcp_connect");
        return -1;
    }
    // 循环读取用户输入
    while (true)
    {
        char cmd[1024];
        if(cmd_read(cmd, sizeof(cmd), "cmd> ") != 0) {
            // 命令读取错误，进行下一次读取
            continue;
        }
        // 解析命令
        char **args = cmd_parse(cmd);
        // 执行命令
        cmd_exec(args);
        // 释放参数数组
        cmd_free(args);
    }
}



