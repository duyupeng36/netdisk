#include <stdlib.h>
#include "message.h"

#include "tcp.h"

int message_recv(int fd, char **buf) {
    // 读取消息的长度
    size_t message_len = 0;
    if(recvn(fd, &message_len, sizeof(size_t)) == -1) {
        return -1;
    }
    *buf = malloc(sizeof(char) * (message_len + 1));
    if(recvn(fd, *buf, message_len) == -1) {
        return -1;
    }
    (*buf)[message_len] = '\0';
    return 0;
}
