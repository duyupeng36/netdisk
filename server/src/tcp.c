#include <unistd.h>

#include "tcp.h"

int tcp_listen(const char *hostname, const char *service) {
    
    struct addrinfo hints = {
        .ai_flags = AI_PASSIVE | AI_NUMERICHOST | AI_NUMERICSERV,
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = 0
    };    

    struct addrinfo *res;

    if(getaddrinfo(hostname, service, &hints, &res) != 0) {
        return -1;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sockfd == -1) {
        freeaddrinfo(res);
        return -1;
    }

    if(bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        freeaddrinfo(res);
        close(sockfd);
        return -1;
    }

    if(listen(sockfd, SOMAXCONN) == -1) {
        freeaddrinfo(res);
        close(sockfd);
        return -1;
    }

    freeaddrinfo(res);
    return sockfd;
}

