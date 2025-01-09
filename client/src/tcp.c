// 库/系统调用的头文件
#include <stddef.h>
#include <unistd.h>


// 自定义头文件
#include <tcp.h>

int tcp_connect(const char *hostname, const char *service) {
    if(hostname == NULL || service == NULL) {
        // 输出日志
        return -1;
    }

    struct addrinfo hints = {
        .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = 0,
    };

    struct addrinfo *res;

    // 解析地址
    if(getaddrinfo(hostname, service, &hints, &res) != 0) {
        // 输出日志
        return -1;
    }

    // 创建套接字
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sockfd == -1) {
        // 输出日志
        freeaddrinfo(res);
        return -1;
    }
    
    // 建立连接
    if(connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        // 输出日志
        close(sockfd);
        freeaddrinfo(res);
        return -1;
    }
    // 释放 addrinfo 结构
    freeaddrinfo(res);
    return sockfd;
}
