#ifndef EPOLL_H 
#define EPOLL_H

#include <sys/types.h>
#include <sys/epoll.h>

/**
 * @brief 将文件描述符 fd 添加到 epoll 实例 epfd 中
 * 
 * @param epfd  epoll 实例
 * @param fd  文件描述符
 * @param events  监听的事件
 * @return int 成功返回 0，失败返回 -1
 */
int epoll_add(int epfd, int fd, int events);
/**
 * @brief 从 epoll 实例 epfd 中删除文件描述符 fd
 * 
 * @param epfd epoll 实例
 * @param fd 文件描述符
 * @return int 成功返回 0，失败返回 -1
 */
int epoll_del(int epfd, int fd);

/**
 * @brief 修改 epoll 实例 epfd 中文件描述符 fd 的监听事件
 * 
 * @param epfd epoll 实例
 * @param fd 文件描述符
 * @param events 新的监听事件
 * @return int 成功返回 0，失败返回 -1
 */
int epoll_mod(int epfd, int fd, int events);

#endif // EPOLL_H