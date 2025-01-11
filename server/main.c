#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/sysinfo.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "config.h"
#include "tcp.h"
#include "epoll.h"

#define CONFIG_PATH "config/config.toml"


int main(int argc, char *argv[]) {

    if(argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s <config file>\n", argv[0]);
        return 1;
    }

    char *config_path = argc > 2 ? argv[1] : CONFIG_PATH;

    // 读取服务端配置文件
    struct server_config server_config;
    server_config.hostname = malloc(sizeof(char) * 16);
    server_config.service = malloc(sizeof(char) * 6); 
    if(load_config(config_path, (struct config *)&server_config, CFG_SERVER) != 0) {
        fprintf(stderr, "Failed to load config\n");
        return 1;
    }

    // 创建线程池


    // 创建监听套接字
    int sockfd = tcp_listen(server_config.hostname, server_config.service);
    if(sockfd == -1) {
        fprintf(stderr, "Failed to create socket\n");
        return 1;
    }

    // 创建 epoll 实例
    int epfd = epoll_create(1024);
    if(epfd == -1) {
        fprintf(stderr, "Failed to create epoll\n");
        return 1;
    }

    // 添加监听套接字到 epoll 实例
    if(epoll_add(epfd, sockfd, EPOLLIN) == -1) {
        fprintf(stderr, "Failed to add sockfd to epoll\n");
        return 1;
    }

    // 监控文件描述符个数
    int monitorNumber = 1;
    // 就像数组大小
    int readySize = 2 * monitorNumber;
    // 创建就绪时间数组
    struct epoll_event *events = malloc(sizeof(struct epoll_event) * readySize);

    while (true)
    {
        int readyNumber = epoll_wait(epfd, events, 2 * monitorNumber, -1);
        if(readyNumber == -1) {
            fprintf(stderr, "Failed to wait for epoll\n");
            return 1;
        }

        // 循环处理事件
        for(int i = 0; i < readyNumber; i++) {
            if(events[i].data.fd == sockfd) {
                // 有新的客户端连接
                int netfd = accept(events[i].data.fd, NULL, NULL);
                if(netfd == -1) {
                    fprintf(stderr, "Failed to accept\n");
                    return 1;
                }
                // 添加新的客户端连接到 epoll 实例
                if(epoll_add(epfd, netfd, EPOLLIN) == -1) {
                    fprintf(stderr, "Failed to add netfd to epoll\n");
                    return 1;
                }
                monitorNumber++;
                if(monitorNumber > readySize) {
                    readySize = 2 * monitorNumber;
                    events = realloc(events, sizeof(struct epoll_event) * readySize);
                    if(events == NULL) {
                        fprintf(stderr, "Failed to realloc events\n");
                        return 1;
                    }
                }
            } else {
                // 客户端发来命令
            }
        }

    }
    

    return 0;
}

