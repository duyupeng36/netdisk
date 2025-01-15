#ifndef TCP_H
#define TCP_H

#include <sys/types.h>
#include <sys/socket.h>
#undef __USE_XOPEN2K
#define __USE_XOPEN2K
#include <netdb.h>

/**
 * @brief 创建一个监听 host:port 的套接字
 * 
 * @param host 本地网卡被分配的 IP 地址
 * @param port 本地端口号
 * @return int 成功返回监听套接字，失败返回 -1
 */
int tcp_listen(const char *host, const char *port);

#endif // TCP_H