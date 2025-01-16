#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "cmd.h"
#include "tcp.h"

const char *command_read(int fd) {
    int size;
    if(recv(fd, &size, sizeof(int), MSG_WAITALL) != sizeof(int)) {
        return NULL;
    }
    char *command = (char *)malloc(size + 1);
    if(command == NULL) {
        return NULL;
    }
    if(recvn(fd, command, size) != size) {
        free(command);
        return NULL;
    }
    command[size] = '\0';
    return command;
}

void command_free(const char *command) {
    free((void *)command);
}

char **command_parse(const char *command, int *argc)
{
    if (command == NULL || argc == NULL)
    {
        return NULL;
    }

    // 跳过前导空白字符串
    while (isspace(*command))
    {
        command++;
    }

    // 计算参数个数
    *argc = 0;

    int i = 0;
    while (command[i] != '\0')
    {
        // 跳过非空白字符
        while (!isspace(command[i]) && command[i] != '\0')
        {
            i++;
        }
        // 参数个数 +1
        (*argc)++;
        // 跳过当前空白字符
        while (isspace(command[i]) && command[i] != '\0')
        {
            i++;
        }
    }
    // printf("argc = %d\n", *argc);
    // 分配参数数组：参数个数 + 1，最后一个参数为 NULL
    char **argv = (char **)malloc((*argc + 1) * sizeof(char *));
    if (argv == NULL)
    {
        return NULL;
    }

    // 解析参数
    i = 0;
    const char *star = command;
    while (*command)
    {
        // 跳过前导非空白字符
        while (!isspace(*command) && *command != '\0')
        {
            command++;
        }
        // 分配存储参数的内存空间
        argv[i] = (char *)malloc((command - star + 1) * sizeof(char));
        // 分配失败
        if (argv[i] == NULL)
        {
            // 释放之前分配的内存
            for (int j = 0; j < *argc; j++)
            {
                free(argv[j]);
            }
            free(argv);
            return NULL;
        }
        // 复制参数
        strncpy(argv[i], star, command - star);
        argv[i][command - star] = '\0';
        // 参数个数 +1
        i++;
        // 跳过当前空白字符
        while (isspace(*command) && *command != '\0')
        {
            command++;
        }
        // 记录下一个参数的起始位置
        star = command;
    }
    argv[i] = NULL;
    return argv;
}

void free_argv(char **argv) {
    if(argv == NULL) {
        return;
    }
    for(int i = 0; argv[i] != NULL; i++) {
        free(argv[i]);
    }
    free(argv);
}