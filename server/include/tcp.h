#ifndef TCP_H
#define TCP_H

/**
 * @brief 创建一个监听 host:port 的套接字
 * 
 * @param host 本地网卡被分配的 IP 地址
 * @param port 本地端口号
 * @return int 成功返回监听套接字，失败返回 -1
 */
int tcp_listen(const char *host, const char *port);

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