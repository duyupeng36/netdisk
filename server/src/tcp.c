#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#undef __USE_XOPEN2K
#define __USE_XOPEN2K
#include <netdb.h>

#include "tcp.h"

int tcp_listen(const char *host, const char *port) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST | AI_NUMERICSERV;
    if(getaddrinfo(host, port, &hints, &res) != 0) {
        return -1;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        return -1;
    }

    // 给套接字绑定 host:port 
    if(bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        close(sockfd);
        return -1;
    }

    // 转为监听套接字
    if(listen(sockfd, 10) == -1) {
        close(sockfd);
        return -1;
    }
    return sockfd;
}

int sendn(int fd, const void *buf, size_t n) {
    ssize_t totalSend = 0;
    const char *data = buf;
    while(totalSend < n) {
        ssize_t numSend = send(fd, data + totalSend, n - totalSend, MSG_NOSIGNAL);
        if (numSend == -1) {
            // 被信号中断
            if(errno == EINTR) {
                continue;
            }
            // 发送失败
            return -1;
        }
        totalSend += numSend;
    }
    return totalSend;
}

int recvn(int fd, void *buf, size_t n) {
    if (buf == NULL || n == 0) {
        return -1;
    }
    ssize_t totalRecv = 0;
    char *data = buf;

    while (totalRecv < n) 
    {
        ssize_t numRecv = recv(fd, data + totalRecv, n - totalRecv, 0);
        if (numRecv == -1) 
        {
            // 被信号中断
            if (errno == EINTR) 
            {
                continue;
            }
            // 接收失败
            return -1;
        }
        totalRecv += numRecv;
    }
    return totalRecv;
}
