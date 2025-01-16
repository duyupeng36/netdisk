#ifndef EXCHANGE_H
#define EXCHANGE_H
#include <stddef.h>

#define MESSAGE_LENGTH 4096

/**
 * @brief 接收消息
 * 
 * @param fd 与服务端通信的文件描述符
 * @param buf 接收消息的缓冲区
 * @return int 成功返回0，失败返回-1
 */
int message_recv(int fd, char **buf);

#endif // EXCHANGE_H
