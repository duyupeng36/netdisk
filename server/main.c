#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include <sys/sysinfo.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "config.h"
#include "tcp.h"
#include "epoll.h"
#include "pool.h"
#include "service.h"
#include "auth.h"

#define CONFIG_PATH "config/config.toml"

int main(int argc, char *argv[])
{

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
    {
        fprintf(stderr, "Usage: %s <config file>\n", argv[0]);
        return 1;
    }

    char *config_path = argc > 2 ? argv[1] : CONFIG_PATH;

    // 读取服务端配置文件
    struct server_config server_config;
    server_config.hostname = malloc(sizeof(char) * 16);
    server_config.service = malloc(sizeof(char) * 6);
    server_config.root = malloc(sizeof(char) * 256);
    if (load_config(config_path, (struct config *)&server_config, CFG_SERVER) != 0)
    {
        fprintf(stderr, "Failed to load config\n");
        return 1;
    }

    // 创建任务队列
    task_queue_t task_queue;
    if (task_queue_init(&task_queue) != 0)
    {
        fprintf(stderr, "Failed to create task queue\n");
        return 1;
    }

    // 创建线程池
    thread_pool_t pool;
    if (thread_pool_init(&pool, server_config.worker_number, &task_queue) != 0)
    {
        fprintf(stderr, "Failed to create thread pool\n");
        return 1;
    }

    // 创建监听套接字
    int sockfd = tcp_listen(server_config.hostname, server_config.service);
    if (sockfd == -1)
    {
        fprintf(stderr, "Failed to create socket\n");
        return 1;
    }

    // 创建 epoll 实例
    int epfd = epoll_create(1024);
    if (epfd == -1)
    {
        fprintf(stderr, "Failed to create epoll\n");
        return 1;
    }

    // 添加监听套接字到 epoll 实例
    if (epoll_add(epfd, sockfd, EPOLLIN) == -1)
    {
        fprintf(stderr, "Failed to add sockfd to epoll\n");
        return 1;
    }

    // 创建用户表
    user_table_t *user_table = user_table_init(1024);
    if (user_table == NULL)
    {
        fprintf(stderr, "Failed to create user table\n");
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
        if (readyNumber == -1)
        {
            fprintf(stderr, "Failed to wait for epoll\n");
            return 1;
        }

        // 循环处理事件
        for (int i = 0; i < readyNumber; i++)
        {
            if (events[i].events & EPOLLRDHUP)
            {
                // 客户端断开连接
                if (epoll_del(epfd, events[i].data.fd) == -1)
                {
                    fprintf(stderr, "Failed to delete fd from epoll\n");
                    return 1;
                }
                close(events[i].data.fd);
                monitorNumber--;
                continue;  // 检查下一个就绪事件
            }
            
            // events[i].events & EPOLLIN
            if (events[i].data.fd == sockfd)
            {
                // 有新的客户端连接
                int netfd = accept(events[i].data.fd, NULL, NULL);
                if (netfd == -1)
                {
                    fprintf(stderr, "Failed to accept\n");
                    return 1;
                }
                // 添加新的客户端到用户表
                if (user_table_add(user_table, netfd) == -1)
                {
                    fprintf(stderr, "Failed to add user to user table\n");
                    return 1;
                }
                // 添加新的客户端连接到 epoll 实例
                if (epoll_add(epfd, netfd, EPOLLIN | EPOLLRDHUP) == -1)
                {
                    fprintf(stderr, "Failed to add netfd to epoll\n");
                    return 1;
                }
                monitorNumber++;
                if (monitorNumber > readySize)
                {
                    readySize = 2 * monitorNumber;
                    struct epoll_event *newEvents = realloc(events, sizeof(struct epoll_event) * readySize);
                    if (newEvents == NULL)
                    {
                        fprintf(stderr, "Failed to realloc events\n");
                        return 1;
                    }
                    events = newEvents;
                }
            }
            else
            {
                // 客户端发来命令
                char **args = NULL;
                int argc = 0;
                if (recv_cmd(events[i].data.fd, &args, &argc) != 0)
                {
                    fprintf(stderr, "Failed to receive command\n");
                    return 1;
                }
                // 检查命令，为命令分配处理函数，并添加到任务队列
                task_handler_t handler = cmd_handler(args[0]);
                if(handler == NULL) {
                    fprintf(stderr, "Invalid command\n");
                    return 1;
                }
                // 从用户表中获取用户
                user_t *user = user_table_get(user_table, events[i].data.fd);
                task_queue_push(&task_queue, user, argc, args, handler);
                // 释放参数
                for (int i = 0; i < argc; i++)
                {
                    free(args[i]);
                }
                free(args);
            }
        }
    }

    return 0;
}
