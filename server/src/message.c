#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "message.h"
#include "tcp.h"

int message_send(int fd, const char *message) {
    size_t len = strlen(message);
    // 发送消息长度
    if(sendn(fd, &len, sizeof(size_t)) != sizeof(size_t)) {
        printf("send message length failed: %s\n", strerror(errno));
        return -1;
    }
    // 发送消息
    if(sendn(fd, message, len) != len) {
        printf("send message failed\n");
        return -1;
    }
    return 0;
}
