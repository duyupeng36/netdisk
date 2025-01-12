
#define _DEFAULT_SOURCE
#include <unistd.h>

#include <string.h>
#include <sys/socket.h>

#include "auth.h"
#include "cmd.h"

int login(int fd) {
    // 发送登录命令
    int argc;
    char **args;
    if((args = cmd_parse("login", &argc)) == NULL) {
        return -1;
    }
    if(cmd_exec(fd, args, argc) == -1) {
        return -1;
    }
    char *password = getpass("Please enter your password: ");
    if(password == NULL) {
        return -1;
    }
    // // 发送密码长度
    // int len = strlen(password);
    // if (send(fd, &len, sizeof(int), 0) != sizeof(int)) {
    //     return -1;
    // }
    // // 发送密码
    // if (send(fd, password, len, 0) != len) {
    //     return -1;
    // }
    return 0;
}