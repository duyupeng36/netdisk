#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/uio.h>
#include <sys/socket.h>

#include "cmd.h"


int cmd_read(char * restrict buf, size_t size, const char * restrict prompt) {
    printf("%s", prompt);
    size_t len = 0;
    while (len < size - 1 && (buf[len] = getchar()) != '\n') {
        len++;
    }
    buf[len] = '\0';
    return len > 0 ? len : -1;
}

char ** cmd_parse(const char *cmd, int *argc) {
    if(cmd == NULL || argc == NULL) {
        return NULL;
    }

    // 跳过前导空白字符串
    while(isspace(*cmd)) {
        cmd++;
    }

    // 计算参数个数
    *argc = 0;

    int i = 0;
    while (cmd[i] != '\0')
    {
        // 跳过非空白字符
        while(!isspace(cmd[i]) && cmd[i] != '\0') {
            i++;
        }
        // 参数个数 +1
        argc++;
        // 跳过当前空白字符
        while(isspace(cmd[i]) && cmd[i] != '\0') {
            i++;
        }
    }
    // printf("argc = %d\n", *argc);
    // 分配参数数组：参数个数 + 1，最后一个参数为 NULL
    char **argv = (char **)malloc((*argc + 1) * sizeof(char *));
    if(argv == NULL) {
        return NULL;
    }
    
    // 解析参数
    int i = 0;
    const char *star = cmd;
    while (*cmd)
    {
        // 跳过前导非空白字符
        while (!isspace(*cmd) && *cmd != '\0') {
            cmd++;
        }
        // 分配存储参数的内存空间
        argv[i] = (char *)malloc((cmd - star + 1) * sizeof(char));
        // 分配失败
        if(argv[i] == NULL) {
            // 释放之前分配的内存
            for(int j = 0; j < argc; j++) {
                free(argv[j]);
            }
            free(argv);
            return NULL;
        }
        // 复制参数
        strncpy(argv[i], star, cmd - star);
        argv[i][cmd - star] = '\0';
        // 参数个数 +1
        i++;
        // 跳过当前空白字符
        while(isspace(*cmd) && *cmd != '\0') {
            cmd++;
        }
        // 记录下一个参数的起始位置
        star = cmd;
    }
    argv[i] = NULL;
    return argv;
}


static bool is_valid_cmd(const char *cmd) {
    if(cmd == NULL) {
        return false;
    }
    if(strcmp(cmd, "ls") == 0) {
        return true;
    }
    if(strcmp(cmd, "cd") == 0) {
        return true;
    }
    if(strcmp(cmd, "pwd") == 0) {
        return true;
    }
    if(strcmp(cmd, "gets") == 0) {
        return true;
    }
    if(strcmp(cmd, "puts") == 0) {
        return true;
    }
    if(strcmp(cmd, "remove") == 0) {
        return true;
    }
    if(strcmp(cmd, "mkdir") == 0) {
        return true;
    }
    if(strcmp(cmd, "rmdir") == 0) {
        return true;
    }
    return false;
}

static int send_cmd(int fd, struct iovec *iov, int iovcnt) {
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));
    msg.msg_iov = iov;
    msg.msg_iovlen = iovcnt;
    return sendmsg(fd, &msg, 0);
}

int cmd_exec(int fd, char **args, int argc){
    if(args == NULL) {
        return -1;
    }

    //检查命令是否符合要求
    if(!is_valid_cmd(args[0])) {
        printf("Invalid command: %s\n", args[0]);
        return -1;
    }

    // 计算每个参数的长度
    int argLenVec[argc];
    for(int i = 0; i < argc; i++) {
        argLenVec[i] = strlen(args[i]);
    }

    // 将每次参数的长度发送给服务端
    if(send(fd, argLenVec, sizeof(argLenVec), 0) != sizeof(argLenVec)) {
        perror("send");
        return -1;
    }
    // 构造 iovec
    struct iovec iov[argc];
    for(int i = 0; i < argc; i++) {
        iov[i].iov_base = args[i];
        iov[i].iov_len = argLenVec[i];
    }
    // 将命令发送给服务端：使用分散写 sendmsg
    if(send_cmd(fd, iov, argc) == -1) {
        perror("send_cmd");
        return -1;
    }
    // 根据命令执行不同的操作
    if(strcmp(args[0], "ls") == 0) {
        // TODO 展示文件列表
    } else if(strcmp(args[0], "cd") == 0) {
        // TODO 切换目录
    } else if(strcmp(args[0], "pwd") == 0) {
        // TODO 显示当前目录
    } else if(strcmp(args[0], "gets") == 0) {
        // TODO 下载文件
    } else if(strcmp(args[0], "puts") == 0) {
        // TODO 上传文件
    } else if(strcmp(args[0], "remove") == 0) {
        // TODO 删除文件
    } else if(strcmp(args[0], "mkdir") == 0) {
        // TODO 创建目录
    } else if(strcmp(args[0], "rmdir") == 0) {
        // TODO 删除目录
    }
}


void cmd_free(char **args) {
    for(char **p = args; *p != NULL; p++) {
        free(*p);
    }
    free(args);
}