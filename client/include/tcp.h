#ifndef TCP_H
#define TCP_H
#include <stddef.h>
/**
 * @brief 连接 hostname 和 service 指定的服务器
 * 
 * @param hostname 服务器的主机名
 * @param service 服务名
 * @return int 返回已连接的套接字描述符，失败返回 -1
 */
int tcp_connect(const char * hostname, const char * service);


/**
 * @brief 发送 n 字节数据，保证发送完所有数据
 * 
 * @param fd 与服务端连接的套接字
 * @param buf 数据缓冲区
 * @param n 数据长度
 * @return int 返回发送的字节数，失败返回 -1
 */
int sendn(int fd, const void *buf, size_t n);

/**
 * @brief 保证接收 n 字节数据
 * 
 * @param fd 与服务端连接的套接字
 * @param buf 数据缓冲区
 * @param n 接收数据的长度
 * @return int 返回接收的字节数，失败返回 -1
 */
int recvn(int fd, void *buf, size_t n);

#endif // TCP_H


