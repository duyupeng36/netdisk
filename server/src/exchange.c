#include <string.h>

#include "exchange.h"
#include "tcp.h"

int message_send(int fd, const char *message) {
    int len = strlen(message);
    // 发送消息长度
    if(sendn(fd, &len, sizeof(len)) == -1) {
        return -1;
    }
    // 发送消息
    if(sendn(fd, message, len) == -1) {
        return -1;
    }
    return 0;
}