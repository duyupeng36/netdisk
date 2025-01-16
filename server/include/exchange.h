
#ifndef EXCHANGE_H
#define EXCHANGE_H

/**
 * @brief 发送消息
 * 
 * @param fd 与客户端通信的套接字
 * @param message 消息字符串
 * @return int 成功返回 0，失败返回 -1
 */
int message_send(int fd, const char *message);

#endif // EXCHANGE_H

