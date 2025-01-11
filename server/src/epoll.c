/* 封装 epoll_ctl */

#include <stddef.h>
#include "epoll.h"


int epoll_add(int epfd, int fd, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        return -1;
    }
    return 0;
}

int epoll_del(int epfd, int fd) {
    if(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        return -1;
    }
    return 0;
}

