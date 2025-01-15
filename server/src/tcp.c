#include <unistd.h>
#include <string.h>

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
