#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "tcp.h"
#include "config.h"
#include "epoll.h"
#include "cmd.h"
#include "task.h"
#include "queue.h"
#include "thread_pool.h"

#define CONFIG_FILE "config/config.toml"

int main(int argc, char *argv[]) {
    if(argc>1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        fprintf(stderr, "Usage: %s <config file path>\n", argv[0]);
        return 0;
    }
    char *config_file = argc > 1 ? argv[1] : CONFIG_FILE;
    // 加载配置文件
    struct server_config server_config;
    if (load_server_config(config_file, &server_config) == -1) {
        fprintf(stderr, "parse config file failed\n");
        return 1;
    }
    // 创建监听套接字
    int sockfd = tcp_listen(server_config.hostname, server_config.service);
    if (sockfd == -1) {
        fprintf(stderr, "create listen socket failed\n");
        return 1;
    }
    printf("server start at %s:%s\n", server_config.hostname, server_config.service);

    // 启用 EPOLL
    int epfd = epoll_create(1024);
    if (epfd == -1) {
        fprintf(stderr, "create epoll failed\n");
        return 1;
    }

    // 添加监听套接字到 epoll
    if(epoll_add(epfd, sockfd, EPOLLIN) == -1) {
        fprintf(stderr, "add listen socket to epoll failed\n");
        return 1;
    }

    // 创建任务队列
    task_queue_t *queue = task_queue_crate(10);  // 创建一个大小为 2^10 的队列
    if(queue == NULL) {
        fprintf(stderr, "create task queue failed\n");
        return 1;
    }
    // 创建线程池
    thread_pool_t *pool = thread_pool_create(server_config.worker_number, queue);
    if(pool == NULL) {
        fprintf(stderr, "create thread pool failed\n");
        return 1;
    }

    int moniterNumber = 1;  // 监听的文件描述符数量

    struct epoll_event *events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * moniterNumber);
    // 开始事件循环
    while (true)
    {
        int readyNumber = epoll_wait(epfd, events, moniterNumber, -1);
        if (readyNumber == -1) {
            fprintf(stderr, "epoll wait failed\n");
            return 1;
        }
        for(int i = 0; i < readyNumber; i++) {
            if(events[i].events & EPOLLRDHUP) {
                // 客户端断开连接
                // 从 epoll 中删除
                if(epoll_del(epfd, events[i].data.fd) == -1) {
                    fprintf(stderr, "delete connection from epoll failed\n");
                }
                // 关闭连接
                close(events[i].data.fd);
                // 处理下一个就绪事件
                continue;
            }
            if(events[i].data.fd == sockfd) {
                // 有新连接
                int connfd = accept(sockfd, NULL, NULL);
                if(connfd == -1) {
                    fprintf(stderr, "accept failed\n");
                    continue;  // 处理下一个就绪事件
                }
                // 添加新连接到 epoll
                if(epoll_add(epfd, connfd, EPOLLIN | EPOLLRDHUP) == -1) {
                    fprintf(stderr, "add connection to epoll failed\n");
                    close(connfd);
                }
            } else {
                // 读取命令字符串
                const char *command = read_command(events[i].data.fd);
                if(command == NULL) {
                    // 命令读取失败：继续处理下一个事件
                    continue;
                }
                // 创建任务
                task_t *task = parse_task(events[i].data.fd, command);
                // 释放命令字符串
                free_command(command);
                // 将任务入队
                if(task_queue_enqueue(queue, task) == -1) {
                    fprintf(stderr, "enqueue task failed\n");
                    free_task(task);
                }
            }
        }
    }
}
