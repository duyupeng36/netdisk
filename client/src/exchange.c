#include <stdlib.h>
#include "exchange.h"

#include "tcp.h"

int message_recv(int fd, char **buf) {
    // 读取消息的长度
    int message_len = 0;
    if(recvn(fd, &message_len, sizeof(message_len)) == -1) {
        return -1;
    }
    *buf = malloc(sizeof(char) * (message_len + 1));
    if(recvn(fd, *buf, message_len) == -1) {
        return -1;
    }
    return 0;
}
